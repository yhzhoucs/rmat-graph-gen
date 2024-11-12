use rmat_graph_gen::generator::Generator;
use criterion::{criterion_group, criterion_main, Criterion};

fn bench_generate() {
    let generator = Generator::new(14, 1 << 18, Some(2333), 0.57, 0.19, 0.19);
    let _ = generator.generate();
}

fn bench_generate_par() {
    let generator = Generator::new(14, 1 << 18, Some(2333), 0.57, 0.19, 0.19);
    let _ = generator.generate_par();
}

fn criterion_benchmark(c: &mut Criterion) {
    let mut group = c.benchmark_group("less samples");
    group.sample_size(10);
    group.bench_function("bench_generator", |b| b.iter(|| bench_generate()));
    group.bench_function("bench_generator_par", |b| b.iter(|| bench_generate_par()));
    group.finish();
}

criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);