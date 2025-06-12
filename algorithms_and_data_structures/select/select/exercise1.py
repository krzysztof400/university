import random
import subprocess
import os

def select_kth_element(generator, size, select_algo, kth):
    """
    This function generates a list of integers using the specified generator,
    then selects the kth smallest element using the specified selection algorithm.
    """
    try:
        # Run generator and capture its output
        generator_result = subprocess.run(
            [generator, str(size)], 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        generated_data = generator_result.stdout

        # Run selection algorithm and capture its output
        select_result = subprocess.run(
            [select_algo, str(kth)], 
            input=generated_data, 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        
    except subprocess.CalledProcessError as e:
        print(f"Error running generator: {e}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None
    
    print(f"Generator: {generator}, Algorithm: {select_algo}, Size: {size}, Kth: {kth}")
    print(f"{select_result.stdout.strip()}")
    print()

def main():
    generators = ["./generator_random", "./generator_ascending", "./generator_descending"]
    algorithms = ["./select", "./randomized_select"]

    for algorithm in algorithms:
        for generator in generators:
            select_kth_element(generator, 1, algorithm, random.randint(1,1))
            select_kth_element(generator, 10, algorithm, random.randint(1,10))
            select_kth_element(generator, 30, algorithm, random.randint(1,30))
            

if __name__ == "__main__":
    main()