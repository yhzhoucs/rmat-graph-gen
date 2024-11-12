use rmat_graph_gen::generator::Generator;
use clap::Parser;
use std::{
    fs::{self, File}, io::{BufWriter, Write}, path::Path
};

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Cli {
    params: Option<Vec<f32>>,
    #[arg(short, long)]
    scale: usize,
    #[arg(short, long, default_value_t = String::from("output.txt"))]
    outfile: String,
    #[arg(long, default_value_t = false)]
    par: bool,
    #[arg(short, long)]
    randseed: Option<u64>,
    #[arg(short, long, default_value_t = 16)]
    degree: usize
}

fn main() {
    let args = Cli::parse();
    let edge_number = (1 << args.scale) * args.degree;

    let (a, b, c) = if let Some(v) = args.params.as_deref() {
        if v.len() != 3 {
            eprintln!("Invalid number of parameters");
            std::process::exit(1);
        }
        (v[0], v[1], v[2])
    } else {
        (0.57, 0.19, 0.19)
    };

    let generator = Generator::new(args.scale, edge_number, args.randseed, a, b, c);
    let edges = if args.par {
        generator.generate_par()
    } else {
        generator.generate()
    };

    println!("Generated {} edges", edges.len());
    let _ = Path::new(&args.outfile)
        .parent().map(|p| match p.exists() {
            false => fs::create_dir_all(p),
            true => Ok(())
        }).expect("Failed to create output directory");
    
    let out = File::options()
        .write(true)
        .read(false)
        .create(true)
        .truncate(true)
        .open(&args.outfile)
        .unwrap();
    let mut writer = BufWriter::new(out);
    for (src, dst) in edges {
        writeln!(writer, "{} {}", src, dst).unwrap();
    }
    writer.flush().unwrap();
}
