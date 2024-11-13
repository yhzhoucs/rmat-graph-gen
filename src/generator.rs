use rand::prelude::*;
use rayon::prelude::*;

pub struct Generator {
    scale: usize,
    edge_number: usize,
    rnd_seed: Option<u64>,
    a: f32,
    b: f32,
    c: f32
}

impl Generator {
    pub fn new(scale: usize, edge_number: usize, rnd_seed: Option<u64>, a: f32, b: f32, c: f32) -> Self {
        Self { scale, edge_number, rnd_seed, a, b, c }
    }

    fn remap(&self, el: &mut Vec<(u64, u64)>) {
        let vertex_number = 1 << self.scale;
        let mut new_ids: Vec<usize> = (0..vertex_number).collect();
        let mut rng = match self.rnd_seed {
            Some(seed) => StdRng::seed_from_u64(seed),
            None => StdRng::from_entropy()
        };
        new_ids.shuffle(&mut rng);
        for i in 0..el.len() {
            el[i].0 = new_ids[el[i].0 as usize] as u64;
            el[i].1 = new_ids[el[i].1 as usize] as u64;
        }
    }

    fn remap_par(&self, el: &mut Vec<(u64, u64)>) {
        let vertex_number = 1 << self.scale;
        let mut new_ids: Vec<usize> = (0..vertex_number).collect();
        let mut rng = match self.rnd_seed {
            Some(seed) => StdRng::seed_from_u64(seed),
            None => StdRng::from_entropy()
        };
        new_ids.shuffle(&mut rng);
        el.par_iter_mut().for_each(|(src, dst)| {
            *src = new_ids[*src as usize] as u64;
            *dst = new_ids[*dst as usize] as u64;
        });
    }

    pub fn generate_par(&self) -> Vec<(u64, u64)> {
        let num_threads = rayon::current_num_threads();
        let chunk_size = 1.max((self.edge_number + num_threads - 1) / num_threads);
        let mut result = (0..self.edge_number).into_par_iter()
            .chunks(chunk_size)
            .enumerate()
            .map(|(chunk_id, chunk)| {
                let mut result: Vec<(u64, u64)> = Vec::with_capacity(chunk.len());
                let mut rng = match self.rnd_seed {
                    Some(seed) => StdRng::seed_from_u64(seed + chunk_id as u64),
                    None => StdRng::from_entropy()
                };
                for _ in chunk {
                    let mut src: u64 = 0;
                    let mut dst: u64 = 0;
                    for _ in 0..self.scale {
                        src = src << 1;
                        dst = dst << 1;
                        let r: f32 = rng.gen();
                        if r < self.a + self.b {
                            if r > self.a {
                                dst += 1;
                            }
                        } else {
                            src += 1;
                            if r > self.a + self.b + self.c {
                                dst += 1;
                            }
                        }
                    }
                    result.push((src, dst));
                }
                result
            })
            .flatten()
            .collect();
        self.remap_par(&mut result);
        result
    }

    pub fn generate(&self) -> Vec<(u64, u64)> {
        let mut result: Vec<(u64, u64)> = Vec::with_capacity(self.edge_number);
        let mut rng = match self.rnd_seed {
            Some(seed) => StdRng::seed_from_u64(seed),
            None => StdRng::from_entropy()
        };
        for _ in 0..self.edge_number {
            let mut src: u64 = 0;
            let mut dst: u64 = 0;
            for _ in 0..self.scale {
                src = src << 1;
                dst = dst << 1;
                let r: f32 = rng.gen();
                if r < self.a + self.b {
                    if r > self.a {
                        dst += 1;
                    }
                } else {
                    src += 1;
                    if r > self.a + self.b + self.c {
                        dst += 1;
                    }
                }
            }
            result.push((src, dst));
        }
        self.remap(&mut result);
        result
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_generate_fix_seed() {
        let generator = Generator::new(8, 1000, Some(2333), 0.57, 0.19, 0.19);
        let edges = generator.generate();
        let another_generator_with_same_seed = Generator::new(8, 1000, Some(2333), 0.57, 0.19, 0.19);
        let another_edges = another_generator_with_same_seed.generate();
        assert_eq!(edges, another_edges);
    }

    #[test]
    fn test_generate_random_seed() {
        let generator = Generator::new(8, 1000, None, 0.57, 0.19, 0.19);
        let edges = generator.generate();
        let another_generator_with_random_seed = Generator::new(8, 1000, None, 0.57, 0.19, 0.19);
        let another_edges = another_generator_with_random_seed.generate();
        assert_ne!(edges, another_edges);
    }

    #[test]
    fn test_generate_par_fix_seed() {
        let generator = Generator::new(8, 1000, Some(2333), 0.57, 0.19, 0.19);
        let edges = generator.generate_par();
        let another_generator_with_same_seed = Generator::new(8, 1000, Some(2333), 0.57, 0.19, 0.19);
        let another_edges = another_generator_with_same_seed.generate_par();
        assert_eq!(edges, another_edges);
    }

    #[test]
    fn test_generate_par_random_seed() {
        let generator = Generator::new(8, 1000, None, 0.57, 0.19, 0.19);
        let edges = generator.generate_par();
        let another_generator_with_random_seed = Generator::new(8, 1000, None, 0.57, 0.19, 0.19);
        let another_edges = another_generator_with_random_seed.generate_par();
        assert_ne!(edges, another_edges);
    }
}