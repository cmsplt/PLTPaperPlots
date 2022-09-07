#!/usr/bin/env bash

condaDir='/afs/cern.ch/work/a/adelanno/miniconda3'

setupConda(){
    url='https://repo.anaconda.com/miniconda/Miniconda3-py39_4.11.0-Linux-x86_64.sh' 
    wget "${url}" --output-document "${condaDir%/*}/${url##*/}"
}

createEnv(){
    # "${condaDir}/condabin/conda" init 'bash'
    source "${condaDir}/etc/profile.d/conda.sh"
    "${condaDir}/bin/conda" env create --file 'conda_env_linux.yml' # name: condaENV
}

activateEnv(){
    source "${condaDir}/etc/profile.d/conda.sh"
    conda activate 'condaENV'
}

createPlots(){
    activateEnv
    for year in 2018; do #2015 2016 2017 2018; do
        for det in 'bcm1f' 'dt' 'hfet' 'hfoc' 'pcc' 'ram'; do
            # python3 run.py --year="${year}" "${det}" 'plt'
            python3 run.py --year="${year}" --energy='13' --lin_summary "${det}" 'plt'
        done
    done
}


_condaInit(){
    # >>> conda initialize >>>
    # !! Contents within this block are managed by 'conda init' !!
    __conda_setup="$("${condaDir}/bin/conda" 'shell.bash' 'hook' 2> /dev/null)"
    if [ $? -eq 0 ]; then
        eval "$__conda_setup"
    else
        if [ -f "${condaDir}/etc/profile.d/conda.sh" ]; then
            . "${condaDir}/etc/profile.d/conda.sh"
        else
            export PATH="${condaDir}/bin:$PATH"
        fi
    fi
    unset __conda_setup
    # <<< conda initialize <<<
}
