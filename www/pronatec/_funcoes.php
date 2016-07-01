<?php

/**
 * Função de listagem de resultados
 */
function filtrar($dados) {
    #ver($dados);
    if ($dados['tipoResultado'] == 'oferta') {
        _resOfertaResumo($dados);
    }
    if ($dados['tipoResultado'] == 'matricula') {
        _resMatriculaResumo($dados);
    }
}

/* Reultado RESUMO para ofertas de Turma */
function _resOfertaResumo($dados) {
    global $db;
    #ver($dados);
    /* Rede Ofertante */
    if (count($dados['rede_ofertante']) > 0 && $dados['rede_ofertante'][0] <> '') {
        $ofertantes = implode(',', $dados['rede_ofertante']);
        $where .= " AND co_rede_ofertante IN ({$ofertantes})";
    }
    /* Status da Oferta (Situação) */
    if (count($dados['status_oferta']) > 0 && $dados['status_oferta'][0] <> '') {
        $status = implode(',', $dados['status_oferta']);
        $where .= " AND co_status_oferta_turma IN ({$status})";
    }
    /* Eixo Tecnológico do Curso */
    if (count($dados['eixo_tecnologico']) > 0 && $dados['eixo_tecnologico'][0] <> '') {
        $eixo_tecnologico = implode(',', $dados['eixo_tecnologico']);
        $where .= " AND co_eixo_tecnologico IN ({$eixo_tecnologico})";
    }
    
    if (count($dados['tipo_curso']) > 0 && $dados['tipo_curso'][0] <> '') {
        $tipo_cursos = implode(',', $dados['tipo_curso']);
        $where .= " AND co_tipo_curso IN ({$tipo_cursos})";
    }

    $sql = "
        SELECT
            '<div class=\"linkTabela\" onclick=\"detalharOfertaPorEstado('''|| otr.sg_uf || ''')\"> + </div>' || otr.sg_uf as sg_uf,
            COUNT(0) as turmas,
            SUM(otr.nu_vagas) AS vagas
        FROM
            pronatec.ofertaresumo otr
        WHERE
            otr.ano_inicio_curso = '{$_SESSION['exercicio']}'
            {$where}
        GROUP BY
            otr.sg_uf
        ORDER BY
            otr.sg_uf ";
    $cabecalho = array("Estado", "Turmas", "Vagas Ofertadas");

    /* Filtro por Estado */
    if (isset($dados['sg_uf']) && $dados['sg_uf'] != 'undefined') {
        $where .= " AND sg_uf = '{$dados['sg_uf']}'";
        $sql = "
        SELECT
            '<div class=\"linkTabela\" onclick=\"detalharOfertaPorMunicipio( '''||otr.sg_uf||''',  '''|| otr.co_municipio || ''')\"> + </div>' || otr.ds_municipio as ds_municipio,
            COUNT(0) as turmas,
            SUM(otr.nu_vagas)   AS vagas
        FROM
            pronatec.ofertaresumo otr
        WHERE
            otr.ano_inicio_curso = '{$_SESSION['exercicio']}'
            {$where}
        GROUP BY
            otr.ds_municipio, otr.co_municipio , otr.sg_uf
        ORDER BY
            otr.ds_municipio ";
        $cabecalho = array("Municipio", "Turmas", "Vagas Ofertadas");
    }
    /* Filtro por Município */
    if (isset($dados['co_municipio']) && $dados['co_municipio'] != 'undefined') {
        $where .= " AND co_municipio = '{$dados['co_municipio']}'";
        $sql = "
            SELECT
                otr.ds_rede_ofertante,                
                ds_unidade_ensino,
                COUNT(0) as turmas,
                SUM(otr.nu_vagas)   AS vagas
            FROM
                pronatec.ofertaresumo otr
            WHERE
                otr.ano_inicio_curso = '{$_SESSION['exercicio']}'
                {$where}
            GROUP BY
                otr.co_unidade_ensino , ds_unidade_ensino, ds_rede_ofertante
            ORDER BY
                otr.ds_rede_ofertante, otr.ds_unidade_ensino ";
        $cabecalho = array("Rede Ofertante","Unidade de Ensino", "Turmas", "Vagas Ofertadas");
    }

    #ver($sql);
    /* Mostra a tabela */
    $db->monta_lista($sql, $cabecalho, 1000, 20, 'S', '', '');
}

/* Reultado RESUMO para matrículas */
function _resMatriculaResumo($dados) {
    global $db;
    #ver($dados);
    /* Programa (Modalidade de Entrada) */
    if (count($dados['programa']) > 0 && $dados['programa'][0] <> '') {
        $programas = implode(',', $dados['programa']);
        $where .= " AND co_tipo_programa IN ({$programas})";
    }
    /* Rede Ofertante */
    if (count($dados['rede_ofertante']) > 0 && $dados['rede_ofertante'][0] <> '') {
        $ofertantes = implode(',', $dados['rede_ofertante']);
        $where .= " AND co_rede_ofertante IN ({$ofertantes})";
    }
    /* Status da Matricula (Situação) */
    if (count($dados['status_matricula']) > 0 && $dados['status_matricula'][0] <> '') {
        $status = implode(',', $dados['status_matricula']);
        $where .= " AND co_tipo_situacao_matricula IN ({$status})";
    }
    /* Eixo Tecnológico do Curso */
    if (count($dados['eixo_tecnologico']) > 0 && $dados['eixo_tecnologico'][0] <> '') {
        $eixo_tecnologico = implode(',', $dados['eixo_tecnologico']);
        $where .= " AND co_eixo_tecnologico IN ({$eixo_tecnologico})";
    }
    
    if (count($dados['tipo_curso']) > 0 && $dados['tipo_curso'][0] <> '') {
        $tipo_cursos = implode(',', $dados['tipo_curso']);
        $where .= " AND co_tipo_curso IN ({$tipo_cursos})";
    }

    $sql = "
        SELECT
            '<div class=\"linkTabela\" onclick=\"detalharMatriculaPorEstado('''|| mtr.sg_uf || ''')\"> + </div>' || mtr.sg_uf as sg_uf,
            COUNT(DISTINCT mtr.co_oferta_turma) as turmas,
           SUM(nu_alunos)   AS alunos
        FROM
            pronatec.matricularesumo mtr
        WHERE
            mtr.ano_inicio_curso = '{$_SESSION['exercicio']}'
            {$where}
        GROUP BY
            mtr.sg_uf
        ORDER BY
            mtr.sg_uf ";
    $cabecalho = array("Estado", "Turmas", "Alunos");

    /* Filtro por Estado */
    if (isset($dados['sg_uf']) && $dados['sg_uf'] != 'undefined') {
        $where .= " AND sg_uf = '{$dados['sg_uf']}'";
        $sql = "
        SELECT
            '<div class=\"linkTabela\" onclick=\"detalharMatriculaPorMunicipio( '''||mtr.sg_uf||''',  '''|| mtr.co_municipio || ''')\"> + </div>' || mtr.ds_municipio as ds_municipio,
             COUNT(DISTINCT mtr.co_oferta_turma) as turmas,
           SUM(nu_alunos)   AS alunos
        FROM
            pronatec.matricularesumo mtr
        WHERE
            mtr.ano_inicio_curso = '{$_SESSION['exercicio']}'
            {$where}
        GROUP BY
            mtr.ds_municipio, mtr.co_municipio , mtr.sg_uf
        ORDER BY
            mtr.ds_municipio ";
        $cabecalho = array("Municipio", "Turmas", "Alunos");
    }
    /* Filtro por Município */
    if (isset($dados['co_municipio']) && $dados['co_municipio'] != 'undefined') {
        $where .= " AND co_municipio = '{$dados['co_municipio']}'";
        $sql = "
            SELECT
                mtr.ds_rede_ofertante,                
                mtr.ds_unidade_ensino,
                COUNT(DISTINCT mtr.co_oferta_turma) as turmas,
                SUM(nu_alunos)   AS alunos
            FROM
                pronatec.matricularesumo mtr
            WHERE
                mtr.ano_inicio_curso = '{$_SESSION['exercicio']}'
                {$where}
            GROUP BY
                mtr.co_unidade_ensino , mtr.ds_unidade_ensino, mtr.ds_rede_ofertante
            ORDER BY
                mtr.ds_rede_ofertante, mtr.ds_unidade_ensino ";
        $cabecalho = array("Rede Ofertante","Unidade de Ensino", "Turmas", "Alunos");
    }

    #ver($sql);
    /* Mostra a tabela */
    $db->monta_lista($sql, $cabecalho, 1000, 20, 'S', '', '');
}
?>