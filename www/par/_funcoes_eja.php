<?php

    #BUSCAR MUNICIPIO BLOQUEADO.
    function bucarMunicipioBloqueado(){
        global $db;

        $sql = "
            SELECT muncodigo 
            FROM eja.municipiobloqueio 
            WHERE munbloqstatus = 'A' AND programa = 'F' AND muncodigo = '{$_SESSION['par']['muncod']}';
        ";
        $muncod = $db->pegaUm($sql);
        return $muncod;
    }

    #BUSCA OS DADOS DO QUESTIONARIO REFERENTE AO MUNICÍPIO OU ESTADO, CASO EXISTA.
    function buscaQuestionarioEJA( $inuid ){
        global $db;

        $sql = "
            SELECT  qejid,
                    usucpf
                    inuid,
                    to_char(qejqejprevinicio, 'DD/MM/YYYY') AS qejqejprevinicio,

                    qejpubliconprioranosiniciais, qejpubliconprioranosfinais,
                    qejpubliconpriorejaqualiprof,
                    qejpubliconpriorejaqualiprof_fund, qejpubliconpriorejaqualiprof_medi,

                    tnp.total_nao_prior,

                    qejpublicoprioritariobraalf,

                    qejcampoanosiniciais, qejcampoanosfinais,
                    qejcampoejaqualiprof,
                    qejcampoejaqualiprof_fund, qejcampoejaqualiprof_medi,

                    tc.total_campo,

                    qejquilombolaanosiniciais, qejquilombolaanosfinais,
                    qejquilombolaqualiprof,
                    qejquilombolaqualiprof_fund, qejquilombolaqualiprof_medi,

                    tq.total_quilombolas,

                    qejindigenasanosiniciais, qejindigenasanosfinais,
                    qejindigenaqualiprof,
                    qejindigenaqualiprof_fund, qejindigenaqualiprof_medi,

                    ti.total_indigenas,

                    qejprivliberanosiniciais, qejprivliberanosfinais,
                    qejprivliberqualiprof,
                    qejprivliberqualiprof_fund, qejprivliberqualiprof_medi,

                    tl.total_liberdade,

                    qejcatmatrecicla,
                    qejcatmatreciclaqtd,
                    qejtrabrural,
                    qejtrabruralqtd,
                    qejpessoarua,
                    qejpessoaruaqtd,

                    qejvlrmecpublnaopri,
                    qejvlrmecegresso,
                    qejvlrmeccomcampo,
                    qejvlrmecquilombolas,
                    qejvlrmecindigenas,
                    qejvlrmecpessoaliberdade,

                    CASE WHEN vmec.qejvlrmec_total = 0
                        THEN NULL
                        ELSE vmec.qejvlrmec_total
                    END AS qejvlrmec_total,

                    CASE WHEN vmec.qejvlrmec_total_geral = 0
                        THEN NULL
                        ELSE vmec.qejvlrmec_total_geral
                    END AS qejvlrmec_total_geral,

                    CASE WHEN vmec.qejvlrmec_valor_liberado = ',00'
                        THEN ''
                        ELSE vmec.qejvlrmec_valor_liberado
                    END AS qejvlrmec_valor_liberado,

                    ( COALESCE(qejpublicoprioritariobraalf,0) + COALESCE(tc.total_campo,0) + COALESCE(tq.total_quilombolas,0) + COALESCE(ti.total_indigenas,0) + COALESCE(tl.total_liberdade,0) ) AS total_prioritario,
                    ( COALESCE(qejpublicoprioritariobraalf,0) + COALESCE(tnp.total_nao_prior,0) + COALESCE(tc.total_campo,0) + COALESCE(tq.total_quilombolas,0) + COALESCE(ti.total_indigenas,0) + COALESCE(tl.total_liberdade,0) ) AS total_geral

            FROM eja.questionarioeja q

            JOIN(
                SELECT 	inuid,
                        CASE WHEN qejpubliconpriorejaqualiprof = 0
                            THEN (qejpubliconprioranosiniciais+qejpubliconprioranosfinais+qejpubliconpriorejaqualiprof_fund+qejpubliconpriorejaqualiprof_medi)
                            ELSE (qejpubliconprioranosiniciais+qejpubliconprioranosfinais+qejpubliconpriorejaqualiprof)
                        END AS total_nao_prior
                FROM eja.questionarioeja
            ) AS tnp ON tnp.inuid = q.inuid

            JOIN(
                SELECT	inuid,
                        CASE WHEN qejcampoejaqualiprof = 0
                            THEN (qejcampoanosiniciais+qejcampoanosfinais+qejcampoejaqualiprof_fund+qejcampoejaqualiprof_medi)
                            ELSE (qejcampoanosiniciais+qejcampoanosfinais+qejcampoejaqualiprof)
                        END AS total_campo
                FROM eja.questionarioeja
            ) AS tc on tc.inuid = q.inuid

            JOIN(
                SELECT  inuid,
                        CASE WHEN qejquilombolaqualiprof = 0
                            THEN (qejquilombolaanosiniciais+qejquilombolaanosfinais+qejquilombolaqualiprof_fund+qejquilombolaqualiprof_medi)
                            ELSE (qejquilombolaanosiniciais+qejquilombolaanosfinais+qejquilombolaqualiprof)
                        END AS total_quilombolas
                FROM eja.questionarioeja
            ) AS tq ON tq.inuid = q.inuid

            JOIN(
                SELECT 	inuid,
                        CASE WHEN qejindigenaqualiprof = 0
                            THEN (qejindigenasanosiniciais+qejindigenasanosfinais+qejindigenaqualiprof_fund+qejindigenaqualiprof_medi)
                            ELSE (qejindigenasanosiniciais+qejindigenasanosfinais+qejindigenaqualiprof)
                        END AS total_indigenas
                FROM eja.questionarioeja
            ) AS ti ON ti.inuid = q.inuid

            JOIN(
                SELECT	inuid,
                        CASE WHEN qejprivliberqualiprof = 0
                            THEN (qejprivliberanosiniciais+qejprivliberanosfinais+qejprivliberqualiprof_fund+qejprivliberqualiprof_medi)
                            ELSE (qejprivliberanosiniciais+qejprivliberanosfinais+qejprivliberqualiprof)
                        END AS total_liberdade
                FROM eja.questionarioeja
            ) AS tl ON tl.inuid = q.inuid

            JOIN(
                SELECT	inuid,
                        ( COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0) ) AS qejvlrmec_total,
                        ( COALESCE(qejvlrmecpublnaopri,0) + COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0) ) AS qejvlrmec_total_geral,
                        TRIM( to_char( ( (COALESCE(qejvlrmecpublnaopri,0) + COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0)) * 1777.38 ), '999G999G999G999D99') ) AS qejvlrmec_valor_liberado
                FROM eja.questionarioeja
            ) AS vmec ON vmec.inuid = q.inuid

            WHERE q.inuid = {$inuid}";

        $dados = $db->pegaLinha($sql);

        return $dados;
    }

    #CARREGA AS MATRICULAS DE REDE PUBLICA MUNICIPIO / ESTADUAL EJA.
    function carregarMatriculasRedPublEJA($tipo){
        global $db;

        if($tipo == 'M'){
            $join = "tab_municipio as mu ON mu.pk_cod_municipio = ent.fk_cod_municipio AND mu.pk_cod_municipio = {$_SESSION['par']['muncod']}";
            $fk_cod_etapa_ensino = "43, 44";
            $id_dependencia_adm = 3;
        }else{
            $join = "tab_estado AS uf ON uf.pk_cod_estado = ent.fk_cod_estado AND uf.sigla = '{$_SESSION['par']['estuf']}'";
            $fk_cod_etapa_ensino = "45";
            $id_dependencia_adm = 2;
        }

        $sql_2010 = "
            SELECT  COUNT(m.fk_cod_entidade) AS total_mat_2010
            FROM educacenso_2010.tab_matricula m
            WHERE   m.fk_cod_mod_ensino = 3 AND m.fk_cod_etapa_ensino IN ( {$fk_cod_etapa_ensino} )
                    AND m.fk_cod_entidade in (
                        SELECT ent.pk_cod_entidade FROM educacenso_2010.tab_entidade AS ent
                        INNER JOIN educacenso_2010.{$join}
                        WHERE ent.id_dependencia_adm = {$id_dependencia_adm})";

        $matricula_2010 = $db->pegaUm($sql_2010);

        $sql_2011 = "
            SELECT  COUNT(m.fk_cod_entidade) AS total_mat_2011
            FROM 	educacenso_2011.tab_matricula m
            WHERE   m.fk_cod_mod_ensino = 3 AND m.fk_cod_etapa_ensino IN ( {$fk_cod_etapa_ensino} )
                    AND m.fk_cod_entidade in (
                        SELECT ent.pk_cod_entidade FROM educacenso_2011.tab_entidade AS ent
                        INNER JOIN educacenso_2011.{$join}
                        WHERE ent.id_dependencia_adm = {$id_dependencia_adm})";

        $matricula_2011 = $db->pegaUm($sql_2011);

        $sql_2012 = "
            SELECT  COUNT(m.fk_cod_entidade) AS total_mat_2012
            FROM 	educacenso_2012.tab_matricula m
            WHERE   m.fk_cod_mod_ensino = 3 AND m.fk_cod_etapa_ensino IN ( {$fk_cod_etapa_ensino} )
                    AND m.fk_cod_entidade in (
                        SELECT ent.pk_cod_entidade FROM educacenso_2012.tab_entidade AS ent
                        INNER JOIN educacenso_2012.{$join}
                        WHERE ent.id_dependencia_adm = {$id_dependencia_adm})";

        $matricula_2012 = $db->pegaUm($sql_2012);

        $sql_2013 = "
            SELECT  COUNT(m.fk_cod_entidade) AS total_mat_2013
            FROM 	educacenso_2013.tab_matricula m
            WHERE   m.fk_cod_mod_ensino = 3 AND m.fk_cod_etapa_ensino IN ( {$fk_cod_etapa_ensino} )
                    AND m.fk_cod_entidade in (
                        SELECT ent.pk_cod_entidade FROM educacenso_2013.tab_entidade AS ent
                        INNER JOIN educacenso_2013.{$join}
                        WHERE ent.id_dependencia_adm = {$id_dependencia_adm})";

        $matricula_2013 = $db->pegaUm($sql_2013);
        
        $sql_2014 = "
            SELECT  COUNT(m.fk_cod_entidade) AS total_mat_2014
            FROM 	educacenso_2014.tab_matricula m
            WHERE   m.fk_cod_mod_ensino = 3 AND m.fk_cod_etapa_ensino IN ( {$fk_cod_etapa_ensino} )
                    AND m.fk_cod_entidade in (
                        SELECT ent.pk_cod_entidade FROM educacenso_2014.tab_entidade AS ent
                        INNER JOIN educacenso_2014.{$join}
                        WHERE ent.id_dependencia_adm = {$id_dependencia_adm})";

        $matricula_2014 = $db->pegaUm($sql_2014);

        $total_matricula['2010'] = $matricula_2010;
        $total_matricula['2011'] = $matricula_2011;
        $total_matricula['2012'] = $matricula_2012;
        $total_matricula['2013'] = $matricula_2013;
        $total_matricula['2014'] = $matricula_2014;

        return $total_matricula;
    }

    #CARREGA AS MATRICULAS DE ENGRESSOS PBA POR MUNICIPIO
    function carregarMatriculaPBA( $tipo ){
        global $db;

        if($tipo == 'M'){
            $where = "muncod = '{$_SESSION['par']['muncod']}'";
        }else{
            $where = "uf = '{$_SESSION['par']['estuf']}'";
        }

        $sql = "
            SELECT  '2010' AS ano,
                    'PREF' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            --WHERE muncod = '{$muncod}' AND ano = '2010' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'
            WHERE $where AND ano = '2010' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'

            UNION

            SELECT  '2010' AS ano,
                    'SECR' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2010' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'SECR'

            UNION

            SELECT  '2011' AS ano,
                    'PREF' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2011' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'

            UNION

            SELECT  '2011' AS ano,
                    'SECR' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2011' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'SECR'

            UNION

            SELECT  '2012' AS ano,
                    'PREF' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2012' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'

            UNION

            SELECT  '2012' AS ano,
                    'SECR' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2012' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'SECR'

            UNION

            SELECT  '2013' AS ano,
                    'PREF' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2013' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'

            UNION

            SELECT  '2013' AS ano,
                    'SECR' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2013' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'SECR'
                
            UNION

            SELECT  '2014' AS ano,
                    'PREF' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2014' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'PREF'

            UNION

            SELECT  '2014' AS ano,
                    'SECR' AS ent_matricula,
                    SUM( CAST( numeromatriculas AS integer ) ) AS numeromatriculas
            FROM eja.dadosmatriculapba
            WHERE $where AND ano = '2014' AND TRIM( SUBSTR( UPPER(entidade), 1, 4 ) ) = 'SECR'

            ORDER BY 1, 2";

        $dados = $db->carregar($sql);

        return $dados;
    }

    function continuaAdesao(){
        $_SESSION['continuaAdesao'] = 'S';
        header("Location:par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A");
        exit();
    }

    # - deletaParecerEJA: DELETA O PARECER DO NOVAS TURMAS EJA - USADO PELA TELA: NOVAS TURMAS EJA - PARECER. (ATUALIZA O STATUS PARA INATIVO - "I")
    function deletaParecerEJA( $dados ){
        global $db;

        $pejid = trim($dados['pejid']);

        if($pejid){
	        $sql = "UPDATE 	eja.parecereja
	                SET 	pejstatus = 'I'
	            	WHERE 	pejid = {$pejid} RETURNING pejid";

	        $return_pejid = $db->pegaUm($sql);
        } else {
            $db->insucesso('Não foi possível excluir o registro!', '', 'principal/programas/feirao_programas/eja/eja_quest_novas_turmas');
        }

        if( $return_pejid > 0 ){
            $db->commit();
            $db->sucesso('principal/programas/feirao_programas/eja/eja_quest_novas_turmas');
        } else {
            $db->insucesso('Não foi possível excluir o registro!', '', 'principal/programas/feirao_programas/eja/eja_quest_novas_turmas');
        }
    }

    # - editarParecerEJA: BUSCA DADOS DO PARECER PARA EDIÇÃO - USADO PELA: TELA NOVAS TURMAS EJA.
    function editarParecerEJA( $dados ){
        global $db;

        $pejid= $dados['pejid'];

        $perfil = pegaPerfilGeral($_SESSION['usucpf']);

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        $sql = "
            SELECT  pejid,
                    usucpf,
                    pejsituacao,
                    pejresposta
            FROM eja.parecereja
            WHERE pejid = {$pejid}
        ";
        $dados = $db->pegaLinha($sql);

        $dados["pejid"] = iconv("ISO-8859-1", "UTF-8", $dados["pejid"]);
        $dados["pejsituacao"] = iconv("ISO-8859-1", "UTF-8", $dados["pejsituacao"]);
        $dados["pejresposta"] = iconv("ISO-8859-1", "UTF-8", $dados["pejresposta"]);

        if( $dados['usucpf'] == $_SESSION['usucpf'] && $estado == WF_EJA_EM_ANALISE_MEC ){
            $dados["permissao"] = iconv("ISO-8859-1", "UTF-8", 'S');
        }else{
            $dados["permissao"] = iconv("ISO-8859-1", "UTF-8", 'N');
        }

        echo simec_json_encode($dados);
        die();
    }


    # - existeParecer: VERIFICA A EXISTENCIA DE PARECER REFERENTE A NOVAS TURMAS EJA  - USADO NA TELA NOVAS TURMAS EJA.
    function existeParecer(){
        global $db ;

        $sql = "
            SELECT  COUNT( pejid ) AS pejid
            FROM eja.parecereja
            WHERE pejstatus = 'A' AND adpid = {$_SESSION['par']['adpid']}
            ORDER BY pejid
        ";
        $pejid = $db->pegaUm($sql);

        if($pejid > 0){
            $existe = 'S';
        }else{
            $existe = 'N';
        }
        return  $existe;
    }

    # - litagemParecerEja: LISTAGEM D E PARECERES - USADO NA TELA NOVAS TURMAS EJA.
    function litagemParecerEja(){
        global $db ;

        $usucpf = $_SESSION['usucpf'];

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarParecerEJA(\''||pejid||'\');\" title=\"Editar parecer\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaParecerEJA(\''||pejid||'\')\" title=\"Deleta o parecer\" >
            </center>
        ";

        $sem = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarParecerEJA(\''||pejid||'\');\" title=\"Editar parecer\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Sem Permissão\" >
            </center>
        ";

        $sql = "
            SELECT  CASE WHEN pe.usucpf = '{$usucpf}' AND w.esdid <> ".WF_EJA_APROVADO."
                        THEN '{$acao}'
                        ELSE '{$sem}'
                    END as acao,
                    pe.pejresposta,
                    CASE WHEN pe.pejsituacao = 't'
                        THEN '<span style=\"color:#00529b;\">Favorável</span>'
                        ELSE '<span style=\"color:red;\">Não Favorável</span>'
                    END AS pejsituacao,
                    replace(to_char(cast(pe.usucpf as bigint), '000:000:000-00'), ':', '.') as usucpf,
                    u.usunome,
                    to_char(pe.pejidtinclusao, 'DD/MM/YYYY') as prcdtinclusao
            FROM eja.parecereja pe
            JOIN seguranca.usuario AS u ON u.usucpf = pe.usucpf
            JOIN par.pfadesaoprograma AS a ON a.adpid = pe.adpid
            JOIN workflow.documento AS w ON w.docid = a.docid
            WHERE pe.pejstatus = 'A' AND pe.adpid = {$_SESSION['par']['adpid']}
            ORDER BY  pejid
        ";
        $cabecalho = Array("Ação", "Descrição", "Situação", "CPF", "Responsável", "Data da Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'right');
        $tamanho = Array('4%', '50%', '10%', '10%', '15%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

    }

    # - permissaoParecer: VERIFICA SE O USUÁRIO TEM A PERMISSÃO PARA INSERÇÃO DO PARECER - USADO NA TELA NOVAS TURMAS EJA.
    function permissaoParecer(){
        global $db;

        $perfil = pegaArrayPerfil( $_SESSION['usucpf'] );

        $programa = pegarProgramaDisponivel();

        if( ( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ANALISTA_EJA, $perfil) ) && in_array(PROGRAMA_EJA, $programa) ){
            $habil['salvar_parec'] = 'onclick="salvarParecerEJA();"';
            $habil['descricao'] = 'S';
            $habil['radio'] = '';
        }else{
            $habil['salvar_parec'] = 'disabled="disabled"';
            $habil['descricao'] = 'N';
            $habil['radio'] = 'disabled="disabled"';
        }
        return $habil;
    }

    # - permissaoPrevisaoOferta: VERIFICA SE O USUÁRIO TEM A PERMISSÃO PARA INSERÇÃO DA OFERTA DE VAGAS PARA EJA - USADO NA TELA NOVAS TURMAS EJA.
    function permissaoPrevisaoOferta(){
        global $db;

        $perfil = pegaArrayPerfil( $_SESSION['usucpf'] );

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        $prog   = pegarProgramaDisponivel();

        $existe_parecer = existeParecer();

        if( in_array(PROGRAMA_EJA, $prog) ){

            if( $estado == WF_EJA_EM_PREENCHIMENTO_UNIDADE ){

                $habilitado['TEXT']  = 'S';
                $habilitado['BOTAO'] = 'S';
                $habilitado['RADIO'] = '';

            }elseif( $estado == WF_EJA_EM_ANALISE_MEC ){

                if( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ANALISTA_EJA, $perfil) ){
                    $habilitado['ANALISE']  = 'S';
                    $habilitado['BOTAO']    = 'S';
                    $habilitado['TEXT']     = 'N';
                    $habilitado['RADIO']    = 'disabled="disabled"';
                }else{
                    $habilitado['ANALISE']  = 'N';
                    $habilitado['BOTAO']    = 'N';
                    $habilitado['TEXT']     = 'N';
                }

            }else{
                $habilitado['ANALISE']  = 'N';
                $habilitado['BOTAO']    = 'N';
                $habilitado['TEXT']     = 'N';
                $habilitado['RADIO']    = 'disabled="disabled"';
            }

            if($estado != WF_EJA_APROVADO){
                if( $existe_parecer == 'S' || $estado != WF_EJA_EM_PREENCHIMENTO_UNIDADE ){

                    if( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ANALISTA_EJA, $perfil) ){
                        $habilitado['PARECER_HAB'] = 'S';
                        $habilitado['BOTAO_PAREC'] = 'onclick="salvarParecerEJA();"';
                        $habilitado['DESCRICAO_P'] = 'S';
                        $habilitado['RADIO_PAREC'] = '';
                    }else{
                        $habilitado['PARECER_HAB'] = 'S';
                        $habilitado['BOTAO_PAREC'] = 'disabled="disabled"';
                        $habilitado['DESCRICAO_P'] = 'N';
                        $habilitado['RADIO_PAREC'] = 'disabled="disabled"';
                    }
                }
            }else{
                $habilitado['PARECER_HAB'] = 'S';
                $habilitado['BOTAO_PAREC'] = 'disabled="disabled"';
                $habilitado['DESCRICAO_P'] = 'N';
                $habilitado['RADIO_PAREC'] = 'disabled="disabled"';
            }

        }else{
            $habilitado['ANALISE']  = 'N';
            $habilitado['BOTAO']    = 'N';
            $habilitado['TEXT']     = 'N';
            $habilitado['RADIO']    = 'disabled="disabled"';
            $habilitado['PARECER_HAB'] = 'N';
            $habilitado['BOTAO_PAREC'] = 'disabled="disabled"';
            $habilitado['DESCRICAO_P'] = 'N';
            $habilitado['RADIO_PAREC'] = 'disabled="disabled"';
        }

        return $habilitado;
    }

    function programaDataHabil( $prgid ){
        global $db;

        $sql = "
            SELECT  pfaid
            FROM par.pfadesao
            WHERE pfadatafinal >= NOW() AND prgid = {$prgid}
            ORDER BY 1
        ";
        $pfaid = $db->pegaUm($sql);

        if( $pfaid > 0 ){
            $programa = 'S';
        }else{
            $programa = 'N';
        }

        return $programa;
    }

    #SALAVA PARECER RELACIONADO COM A ADESÃO AO PROGRAMA.
    function salvarParecerEJA( $dados ){
        global $db;

        $pejid       = $dados['pejid'];
        $usucpf      = $_SESSION['usucpf'];
        $adpid       = $_SESSION['par']['adpid'];
        $pejresposta = substr(trim( addslashes( $dados['pejresposta'] ) ), 0, 1000);
        $pejsituacao = $dados['pejsituacao'] == 'S' ? 't' : 'f';

        if(empty($pejid)){
            $sql = "INSERT INTO eja.parecereja(
                        usucpf,
                        adpid,
                        pejsituacao,
                        pejresposta
                    )VALUES(
                        '{$usucpf}',
                        {$adpid},
                        '{$pejsituacao}',
                        '{$pejresposta}'
                ) RETURNING pejid;";
        } else {
            $sql = "UPDATE 	eja.parecereja
                    SET 	pejsituacao = '{$pejsituacao}',
                       		pejresposta = '{$pejresposta}'
                 	WHERE 	pejid = {$pejid} RETURNING pejid;";
        }
        $return_pejid = $db->pegaUm($sql);

        if($return_pejid > 0){
            $db->commit();
            $db->sucesso('principal/programas/feirao_programas/eja/eja_quest_novas_turmas');
        } else {
            $db->insucesso('Não foi possível gravar o registro!', '', 'principal/programas/feirao_programas/eja/eja_quest_novas_turmas&acao=A');
        }
    }

    #SALVA DADOS DO QUESTIONÁRIO DE ADESÃO DO FINANCIAMENTO DO EJA.
    function salvarQuestEJA( $dados ){
        global $db;

        extract($dados);

        $inuid  = $_SESSION['par']['inuid'];
        $usucpf = $_SESSION['usucpf'];

        $qejqejprevinicio = formata_data_sql( $dados['qejqejprevinicio'] );

        $qejpubliconprioranosiniciais       = $qejpubliconprioranosiniciais ? $qejpubliconprioranosiniciais : '0';
        $qejpubliconprioranosfinais         = $qejpubliconprioranosfinais ? $qejpubliconprioranosfinais : '0';
        $qejpubliconpriorejaqualiprof       = $qejpubliconpriorejaqualiprof ? $qejpubliconpriorejaqualiprof : '0';
        $qejpubliconpriorejaqualiprof_fund  = $qejpubliconpriorejaqualiprof_fund ? $qejpubliconpriorejaqualiprof_fund : '0';
        $qejpubliconpriorejaqualiprof_medi  = $qejpubliconpriorejaqualiprof_medi ? $qejpubliconpriorejaqualiprof_medi : '0';
        $qejpublicoprioritariobraalf        = $qejpublicoprioritariobraalf ? $qejpublicoprioritariobraalf : '0';
        $qejcampoanosiniciais               = $qejcampoanosiniciais ? $qejcampoanosiniciais : '0';
        $qejcampoanosfinais                 = $qejcampoanosfinais ? $qejcampoanosfinais : '0';
        $qejcampoejaqualiprof               = $qejcampoejaqualiprof ? $qejcampoejaqualiprof : '0';
        $qejcampoejaqualiprof_fund          = $qejcampoejaqualiprof_fund ? $qejcampoejaqualiprof_fund : '0';
        $qejcampoejaqualiprof_medi          = $qejcampoejaqualiprof_medi ? $qejcampoejaqualiprof_medi : '0';
        $qejquilombolaanosiniciais          = $qejquilombolaanosiniciais ? $qejquilombolaanosiniciais : '0';
        $qejquilombolaanosfinais            = $qejquilombolaanosfinais ? $qejquilombolaanosfinais : '0';
        $qejquilombolaqualiprof             = $qejquilombolaqualiprof ? $qejquilombolaqualiprof : '0';
        $qejquilombolaqualiprof_fund        = $qejquilombolaqualiprof_fund ? $qejquilombolaqualiprof_fund : '0';
        $qejquilombolaqualiprof_medi        = $qejquilombolaqualiprof_medi ? $qejquilombolaqualiprof_medi : '0';
        $qejindigenasanosiniciais           = $qejindigenasanosiniciais ? $qejindigenasanosiniciais : '0';
        $qejindigenasanosfinais             = $qejindigenasanosfinais ? $qejindigenasanosfinais : '0';
        $qejindigenaqualiprof               = $qejindigenaqualiprof ? $qejindigenaqualiprof : '0';
        $qejindigenaqualiprof_fund          = $qejindigenaqualiprof_fund ? $qejindigenaqualiprof_fund : '0';
        $qejindigenaqualiprof_medi          = $qejindigenaqualiprof_medi ? $qejindigenaqualiprof_medi : '0';
        $qejprivliberanosiniciais           = $qejprivliberanosiniciais ? $qejprivliberanosiniciais : '0';
        $qejprivliberanosfinais             = $qejprivliberanosfinais ? $qejprivliberanosfinais : '0';
        $qejprivliberqualiprof              = $qejprivliberqualiprof ? $qejprivliberqualiprof : '0';
        $qejprivliberqualiprof_fund         = $qejprivliberqualiprof_fund ? $qejprivliberqualiprof_fund : '0';
        $qejprivliberqualiprof_medi         = $qejprivliberqualiprof_medi ? $qejprivliberqualiprof_medi : '0';

        $qejvlrmecpublnaopri        = $qejvlrmecpublnaopri ? $qejvlrmecpublnaopri : 'NULL';
        $qejvlrmecegresso           = $qejvlrmecegresso ? $qejvlrmecegresso : 'NULL';
        $qejvlrmeccomcampo          = $qejvlrmeccomcampo ? $qejvlrmeccomcampo : 'NULL';
        $qejvlrmecquilombolas       = $qejvlrmecquilombolas ? $qejvlrmecquilombolas : 'NULL';
        $qejvlrmecindigenas         = $qejvlrmecindigenas ? $qejvlrmecindigenas : 'NULL';
        $qejvlrmecpessoaliberdade   = $qejvlrmecpessoaliberdade ? $qejvlrmecpessoaliberdade : 'NULL';

        $qejcatmatrecicla           = $qejcatmatrecicla == 'S' ? 't' : 'f';
        $qejcatmatreciclaqtd        = $qejcatmatreciclaqtd ? $qejcatmatreciclaqtd : '0';

        $qejtrabrural               = $qejtrabrural == 'S' ? 't' : 'f';
        $qejtrabruralqtd            = $qejtrabruralqtd ? $qejtrabruralqtd : '0';

        $qejpessoarua               = $qejpessoarua == 'S' ? 't' : 'f';
        $qejpessoaruaqtd            = $qejpessoaruaqtd ? $qejpessoaruaqtd : '0';
/*
        if($media <= $total_geral){
            $texto = 'Quantidade de matrícula ultrapassa a média dos anos (2010,2011,2012 e 2013) = '.($media/2).'!';
            $db->insucesso($texto, '', 'principal/programas/feirao_programas/eja/eja_quest_novas_turmas&acao=A');
        }
*/
        if( $qejid == '' ){
            $sql = "
                INSERT INTO eja.questionarioeja(
                    usucpf, inuid,
                    qejqejprevinicio,
                    qejpubliconprioranosiniciais, qejpubliconprioranosfinais,
                    qejpubliconpriorejaqualiprof,
                    qejpubliconpriorejaqualiprof_fund, qejpubliconpriorejaqualiprof_medi,
                    qejpublicoprioritariobraalf,
                    qejcampoanosiniciais, qejcampoanosfinais,
                    qejcampoejaqualiprof,
                    qejcampoejaqualiprof_fund, qejcampoejaqualiprof_medi,
                    qejquilombolaanosiniciais, qejquilombolaanosfinais,
                    qejquilombolaqualiprof,
                    qejquilombolaqualiprof_fund, qejquilombolaqualiprof_medi,
                    qejindigenasanosiniciais, qejindigenasanosfinais,
                    qejindigenaqualiprof,
                    qejindigenaqualiprof_fund, qejindigenaqualiprof_medi,
                    qejprivliberanosiniciais, qejprivliberanosfinais,
                    qejprivliberqualiprof,
                    qejprivliberqualiprof_fund, qejprivliberqualiprof_medi,
                    qejcatmatrecicla,
                    qejcatmatreciclaqtd,
                    qejtrabrural,
                    qejtrabruralqtd,
                    qejpessoarua,
                    qejpessoaruaqtd,
                    qejvlrmecpublnaopri,
                    qejvlrmecegresso,
                    qejvlrmeccomcampo,
                    qejvlrmecquilombolas,
                    qejvlrmecindigenas,
                    qejvlrmecpessoaliberdade
                )VALUES(
                    '{$usucpf}',
                    {$inuid},
                    '{$qejqejprevinicio}',
                    {$qejpubliconprioranosiniciais},
                    {$qejpubliconprioranosfinais},
                    {$qejpubliconpriorejaqualiprof},
                    {$qejpubliconpriorejaqualiprof_fund},
                    {$qejpubliconpriorejaqualiprof_medi},
                    {$qejpublicoprioritariobraalf},
                    {$qejcampoanosiniciais},
                    {$qejcampoanosfinais},
                    {$qejcampoejaqualiprof},
                    {$qejcampoejaqualiprof_fund},
                    {$qejcampoejaqualiprof_medi},
                    {$qejquilombolaanosiniciais},
                    {$qejquilombolaanosfinais},
                    {$qejquilombolaqualiprof},
                    {$qejquilombolaqualiprof_fund},
                    {$qejquilombolaqualiprof_medi},
                    {$qejindigenasanosiniciais},
                    {$qejindigenasanosfinais},
                    {$qejindigenaqualiprof},
                    {$qejindigenaqualiprof_fund},
                    {$qejindigenaqualiprof_medi},
                    {$qejprivliberanosiniciais},
                    {$qejprivliberanosfinais},
                    {$qejprivliberqualiprof},
                    {$qejprivliberqualiprof_fund},
                    {$qejprivliberqualiprof_medi},
                    '{$qejcatmatrecicla}',
                    {$qejcatmatreciclaqtd},
                    '{$qejtrabrural}',
                    {$qejtrabruralqtd},
                    '{$qejpessoarua}',
                    {$qejpessoaruaqtd},
                    {$qejvlrmecpublnaopri},
                    {$qejvlrmecegresso},
                    {$qejvlrmeccomcampo},
                    {$qejvlrmecquilombolas},
                    {$qejvlrmecindigenas},
                    {$qejvlrmecpessoaliberdade}
                ) RETURNING qejid;
            ";
        } else {
            $sql = "
                UPDATE eja.questionarioeja
                    SET usucpf                              = '{$usucpf}',
                        inuid                               = {$inuid},
                        qejqejprevinicio                    = '{$qejqejprevinicio}',
                        qejpubliconprioranosiniciais        = {$qejpubliconprioranosiniciais},
                        qejpubliconprioranosfinais          = {$qejpubliconprioranosfinais},
                        qejpubliconpriorejaqualiprof        = {$qejpubliconpriorejaqualiprof},
                        qejpubliconpriorejaqualiprof_fund   = {$qejpubliconpriorejaqualiprof_fund},
                        qejpubliconpriorejaqualiprof_medi   = {$qejpubliconpriorejaqualiprof_medi},
                        qejpublicoprioritariobraalf         = {$qejpublicoprioritariobraalf},
                        qejcampoanosiniciais                = {$qejcampoanosiniciais},
                        qejcampoanosfinais                  = {$qejcampoanosfinais},
                        qejcampoejaqualiprof                = {$qejcampoejaqualiprof},
                        qejcampoejaqualiprof_fund           = {$qejcampoejaqualiprof_fund},
                        qejcampoejaqualiprof_medi           = {$qejcampoejaqualiprof_medi},
                        qejquilombolaanosiniciais           = {$qejquilombolaanosiniciais},
                        qejquilombolaanosfinais             = {$qejquilombolaanosfinais},
                        qejquilombolaqualiprof              = {$qejquilombolaqualiprof},
                        qejquilombolaqualiprof_fund         = {$qejquilombolaqualiprof_fund},
                        qejquilombolaqualiprof_medi         = {$qejquilombolaqualiprof_medi},
                        qejindigenasanosiniciais            = {$qejindigenasanosiniciais},
                        qejindigenasanosfinais              = {$qejindigenasanosfinais},
                        qejindigenaqualiprof                = {$qejindigenaqualiprof},
                        qejindigenaqualiprof_fund           = {$qejindigenaqualiprof_fund},
                        qejindigenaqualiprof_medi           = {$qejindigenaqualiprof_medi},
                        qejprivliberanosiniciais            = {$qejprivliberanosiniciais},
                        qejprivliberanosfinais              = {$qejprivliberanosfinais},
                        qejprivliberqualiprof               = {$qejprivliberqualiprof},
                        qejprivliberqualiprof_fund          = {$qejprivliberqualiprof_fund},
                        qejprivliberqualiprof_medi          = {$qejprivliberqualiprof_medi},
                        qejcatmatrecicla                    = '{$qejcatmatrecicla}',
                        qejcatmatreciclaqtd                 = {$qejcatmatreciclaqtd},
                        qejtrabrural                        = '{$qejtrabrural}',
                        qejtrabruralqtd                     = {$qejtrabruralqtd},
                        qejpessoarua                        = '{$qejpessoarua}',
                        qejpessoaruaqtd                     = {$qejpessoaruaqtd},
                        qejvlrmecpublnaopri                 = {$qejvlrmecpublnaopri},
                        qejvlrmecegresso                    = {$qejvlrmecegresso},
                        qejvlrmeccomcampo                   = {$qejvlrmeccomcampo},
                        qejvlrmecquilombolas                = {$qejvlrmecquilombolas},
                        qejvlrmecindigenas                  = {$qejvlrmecindigenas},
                        qejvlrmecpessoaliberdade            = {$qejvlrmecpessoaliberdade}
                WHERE qejid = {$qejid} RETURNING qejid;";
        }
        $return_qejid = $db->pegaUm($sql);

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        if($return_qejid > 0){
            $db->commit();

            if($estado == WF_EJA_EM_PREENCHIMENTO_UNIDADE){
                $db->sucesso('principal/programas/feirao_programas/eja/eja_lista_escola', '', 'Operação realizada com sucesso! Preencha a lista de Escolas.');
            } else {
                $db->sucesso('principal/programas/feirao_programas/eja/eja_quest_novas_turmas', '', 'Operação realizada com sucesso!');
            }
        } else {
            $db->insucesso('Não foi possível gravar o registro!', '', 'principal/programas/feirao_programas/eja/eja_lista_escola&acao=A');
        }
    }

    function verificaRegrasAlfabetizado(){
        global $db;

        if( $_SESSION['par']['estuf'] != '' ){
            $and = " AND pbauf = '{$_SESSION['par']['estuf']}' ";
        }else{
            $and = " AND pbamuncod = '{$_SESSION['par']['muncod']}' ";
        }

        $sql ="
            SELECT COUNT(pbaanoexercicio) AS existe_sba
            FROM eja.dadospba
            WHERE pbaanoexercicio in ('2010', '2011', '2012') {$and}
        ";
        $dados = $db->pegaUm($sql);

        if( $dados > 0 ){
            $existe_sba = 'S';
        }else{
            $existe_sba = 'N';
        }
        return $existe_sba;
    }

    function verificaRegrasIndiginas(){
        global $db;

        if( $_SESSION['par']['estuf'] != '' ){
            $and = " AND sigla = '{$_SESSION['par']['estuf']}' ";
        }else{
            $and = " AND ee.fk_cod_municipio = '{$_SESSION['par']['muncod']}' ";
        }

        $sql = "
            SELECT COUNT(id_educacao_indigena) AS id_educacao_indigena
            FROM ".SCHEMAEDUCACENSO.".tab_dado_escola te

            INNER JOIN ".SCHEMAEDUCACENSO.".tab_entidade AS ee ON ee.pk_cod_entidade = te.fk_cod_entidade
            INNER JOIN ".SCHEMAEDUCACENSO.".tab_municipio AS tm ON tm.pk_cod_municipio = ee.fk_cod_municipio
            INNER JOIN ".SCHEMAEDUCACENSO.".tab_estado AS es ON es.pk_cod_estado = tm.fk_cod_estado

            WHERE id_educacao_indigena = 1 {$and}
        ";
        $dados = $db->pegaUm($sql);

        if( $dados > 0 ){
            $existe_indigina = 'S';
        }else{
            $existe_indigina = 'N';
        }
        return $existe_indigina;

    }

    function verificaRegrasQuilombola(){
        global $db;
        
        if( $_SESSION['par']['estuf'] != '' ){
            $and = " AND sigla = '{$_SESSION['par']['estuf']}' ";
        }else{
            $and = " AND ee.fk_cod_municipio = '{$_SESSION['par']['muncod']}' ";
        }
        
        $sql = "
            SELECT COUNT(fk_localizacao_diferenciada) AS fk_localizacao_diferenciada
            FROM ".SCHEMAEDUCACENSO.".tab_dado_escola te

            INNER JOIN ".SCHEMAEDUCACENSO.".tab_entidade AS ee ON ee.pk_cod_entidade = te.fk_cod_entidade
            INNER JOIN ".SCHEMAEDUCACENSO.".tab_municipio AS tm ON tm.pk_cod_municipio = ee.fk_cod_municipio
            INNER JOIN ".SCHEMAEDUCACENSO.".tab_estado AS es ON es.pk_cod_estado = tm.fk_cod_estado

            WHERE fk_localizacao_diferenciada = 3 {$and}
        ";
        $dados = $db->pegaUm($sql);

        if( $dados > 0 ){
            $existe_indigina = 'S';
        }else{
            $existe_indigina = 'N';
        }
        return $existe_indigina;
    }


    function dadosImpressaoTermoAdsao(){

        $dadosQuest = buscaQuestionarioEJA( $_SESSION['par']['inuid'] );

        $existeAlfabetizado = verificaRegrasAlfabetizado();
        $existeQuilombola = verificaRegrasQuilombola();
        $existeIndigina = verificaRegrasIndiginas();

        if( $_SESSION['par']['muncod'] == '' ){
            #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
            $unid_1         = 'estado';
            $unid_2         = 'Unidade da Federação não identificada';
            $muncod_estuf   = 'UF';
            $colspan        = '8';
            $descricao_rede = 'estadual';

            $num_mat = carregarMatriculaPBA('E');

            if($_SESSION['total_matricula']['controle'] != 'S'){
                $total_matricula    = carregarMatriculasRedPublEJA('E');

                $_SESSION['total_matricula']['controle'] = 'S';
                $_SESSION['total_matricula']['2010'] = $total_matricula['2010'];
                $_SESSION['total_matricula']['2011'] = $total_matricula['2011'];
                $_SESSION['total_matricula']['2012'] = $total_matricula['2012'];
                $_SESSION['total_matricula']['2013'] = $total_matricula['2013'];

            } else {
                $total_matricula['2010'] = $_SESSION['total_matricula']['2010'];
                $total_matricula['2011'] = $_SESSION['total_matricula']['2011'];
                $total_matricula['2012'] = $_SESSION['total_matricula']['2012'];
                $total_matricula['2013'] = $_SESSION['total_matricula']['2013'];
            }
        } else {
            #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
            $unid_1         = 'município';
            $unid_2         = 'Município não identificado';
            $muncod_estuf   = 'M';
            $colspan        = '6';
            $descricao_rede = 'municipal';

            $num_mat = carregarMatriculaPBA('M');

            if( $_SESSION['total_matricula']['controle'] != 'S' ){
                $total_matricula = carregarMatriculasRedPublEJA('M');

                $_SESSION['total_matricula']['controle'] = 'S';
                $_SESSION['total_matricula']['2010'] = $total_matricula['2010'];
                $_SESSION['total_matricula']['2011'] = $total_matricula['2011'];
                $_SESSION['total_matricula']['2012'] = $total_matricula['2012'];
                $_SESSION['total_matricula']['2013'] = $total_matricula['2013'];
            } else {
                $total_matricula['2010'] = $_SESSION['total_matricula']['2010'];
                $total_matricula['2011'] = $_SESSION['total_matricula']['2011'];
                $total_matricula['2012'] = $_SESSION['total_matricula']['2012'];
                $total_matricula['2013'] = $_SESSION['total_matricula']['2013'];
            }
        }

        $docid = pgCriarDocumento($_SESSION['par']['adpid']);
        $estado = pgPegarEstadoAtual($docid);
        $quest = verificaPreenchimentoQuestionarioEJA();

        if( $estado != WF_EJA_EM_PREENCHIMENTO_UNIDADE ){
            if( $quest ){
?>
                <table align="center"border="1" class="tabela" cellpadding="3" cellspacing="1" style="font-size:8px; text-align: right;">
                    <tr>
                        <td class ="SubTituloDireita" width="14%">Previsão de Início:</td>
                        <td width="86%" colspan="<?=$colspan+1?>" style="text-align: left;">
                            <?php
                                echo $qejqejprevinicio = $dadosQuest['qejqejprevinicio'];
                            ?>
                        </td>
                        <td width="2%" class ="SubTituloDireitaMenor">Total MEC</td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita">Matrícula público não prioritário: Anos Iniciais</td>
                        <td width="3%">
                            <?php
                                echo $qejpubliconprioranosiniciais = $dadosQuest['qejpubliconprioranosiniciais'];
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor" width="5%">Anos Finais:</td>
                        <td width="4%">
                            <?php
                                echo $qejpubliconprioranosfinais = $dadosQuest['qejpubliconprioranosfinais'];
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor" width="10%">EJA Integrada a qualificação profissional:</td>
                                <td width="3%">
                                    <?php
                                        echo $qejpubliconpriorejaqualiprof = $dadosQuest['qejpubliconpriorejaqualiprof'];
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor" width="10%">EJA Integrada a qualificação profissional/fundamental:</td>
                                <td width="3%">
                                    <?php
                                        echo $qejpubliconpriorejaqualiprof_fund = $dadosQuest['qejpubliconpriorejaqualiprof_fund'];
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor" width="13%">EJA Integrada a qualificação profissional/médio:</td>
                                <td width="3%">
                                    <?php
                                        echo $qejpubliconpriorejaqualiprof_medi = $dadosQuest['qejpubliconpriorejaqualiprof_medi'];
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor" width="2%">Total:</td>
                        <td width="4%">
                            <?php
                                echo $total_nao_prior = $dadosQuest['total_nao_prior'];
                            ?>
                        </td>
                        <td align="right">
                            <?php
                                echo $dadosQuest['qejvlrmecpublnaopri'] ? $dadosQuest['qejvlrmecpublnaopri'] : '-';
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="10" class ="SubTituloEsquerda" style="text-align: left;">Matrícula público prioritário. Modalidades:</td>
                    </tr>

                    <!--INÍCIO PERGUNTA A-->
                    <tr>
                        <td class ="SubTituloDireita">A - Egresso do Programa Brasil Alfabetizado:</td>
                            <?php
                                if($existeAlfabetizado == 'S'){
                                    echo "<td>";
                                    echo $qejpublicoprioritariobraalf = $dadosQuest['qejpublicoprioritariobraalf'];
                                    echo "</td>";
                                    echo "<td colspan=\"{$colspan}\" class =\"SubTituloDireitaMenor\">&nbsp;</td>";

                                    echo "<td align=\"right\">";
                                        echo $dadosQuest['qejvlrmecegresso'] ? $dadosQuest['qejvlrmecegresso'] : '-';
                                    echo "</td>";

                                }else{
                                    echo "<td colspan=\"9\" class=\"SubTituloEsquerdaMenor\" style=\"text-align: left; color: red;\"> <b> - Opçao permitida apenas para os {$unid_1} que realizaram adesão em pelo menos um dos seguintes anos: 2010, 2011 e 2012.</b> </td>";
                                }
                            ?>
                        </td>
                    </tr>
                    <!--FIM PERGUNTA A-->

                    <!--INÍCIO PERGUNTA B-->
                    <tr>
                        <td class ="SubTituloDireita">B - Estudades das comunidades do Campo: Anos Iniciais</td>
                        <td>
                            <?php
                                echo $qejcampoanosiniciais = $dadosQuest['qejcampoanosiniciais'];
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                        <td>
                            <?php
                                echo $qejcampoanosfinais = $dadosQuest['qejcampoanosfinais'];
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                <td>
                                    <?php
                                        echo $qejcampoejaqualiprof = $dadosQuest['qejcampoejaqualiprof'];
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                <td>
                                    <?php
                                        echo $qejcampoejaqualiprof_fund = $dadosQuest['qejcampoejaqualiprof_fund'];
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                <td>
                                    <?php
                                        echo $qejcampoejaqualiprof_medi = $dadosQuest['qejcampoejaqualiprof_medi'];
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor">Total:</td>
                        <td>
                            <?php
                                echo $total_campo = $dadosQuest['total_campo'];
                            ?>
                        </td>
                        <td align="right">
                            <?php
                                echo $dadosQuest['qejvlrmeccomcampo'] ? $dadosQuest['qejvlrmeccomcampo'] : '-';
                            ?>
                        </td>
                    </tr>
                    <!--FIM PERGUNTA B-->

                    <!--INÍCIO PERGUNTA C-->
                    <tr>
                        <td class ="SubTituloDireita">C - Quilombolas: Anos Iniciais</td>
                        <?  if($existeQuilombola == 'S'){ ?>
                                <td>
                                    <?php
                                        echo $qejquilombolaanosiniciais = $dadosQuest['qejquilombolaanosiniciais'];
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                                <td>
                                    <?php
                                        echo $qejquilombolaanosfinais = $dadosQuest['qejquilombolaanosfinais'];
                                    ?>
                                </td>

                                <? if($muncod_estuf == 'M'){ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                        <td>
                                            <?php
                                                echo $qejquilombolaqualiprof = $dadosQuest['qejquilombolaqualiprof'];
                                            ?>
                                        </td>
                                <? }else{ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                        <td>
                                            <?php
                                                echo $qejquilombolaqualiprof_fund = $dadosQuest['qejquilombolaqualiprof_fund'];
                                            ?>
                                        </td>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                        <td>
                                            <?php
                                                echo $qejquilombolaqualiprof_medi = $dadosQuest['qejquilombolaqualiprof_medi'];
                                            ?>
                                        </td>
                                <? } ?>

                                <td class ="SubTituloDireitaMenor">Total:</td>
                                <td>
                                    <?php
                                        echo $total_quilombolas = $dadosQuest['total_quilombolas'];
                                    ?>
                                </td>
                                <td align="right">
                                    <?php
                                        echo $dadosQuest['qejvlrmecquilombolas'] ? $dadosQuest['qejvlrmecquilombolas'] : '-';
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td colspan="9" class="SubTituloEsquerdaMenor" style="text-align: left; color: red;"> <b> - <?=$unid_2;?> no censo escolar com atendimento aos Quilombolas.</b></td>
                        <? } ?>
                    </tr>
                    <!--FIM PERGUNTA C-->

                    <!--INÍCIO PERGUNTA D-->
                    <tr>
                        <td class ="SubTituloDireita">D - Indígenas: Anos Iniciais</td>
                        <? if($existeIndigina == 'S'){ ?>
                                <td>
                                    <?php
                                        echo $qejindigenasanosiniciais = $dadosQuest['qejindigenasanosiniciais'];
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                                <td>
                                    <?php
                                        echo $qejindigenasanosfinais = $dadosQuest['qejindigenasanosfinais'];
                                    ?>
                                </td>

                                <? if($muncod_estuf == 'M'){ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                        <td>
                                            <?php
                                                echo $qejindigenaqualiprof = $dadosQuest['qejindigenaqualiprof'];
                                            ?>
                                        </td>
                                <? }else{ ?>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                        <td>
                                            <?php
                                                echo $qejindigenaqualiprof_fund = $dadosQuest['qejindigenaqualiprof_fund'];
                                            ?>
                                        </td>
                                        <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                        <td>
                                            <?php
                                                echo $qejindigenaqualiprof_medi = $dadosQuest['qejindigenaqualiprof_medi'];
                                            ?>
                                        </td>
                                <? } ?>

                                <td class ="SubTituloDireitaMenor">Total:</td>
                                <td>
                                    <?php
                                        echo $total_indigenas = $dadosQuest['total_indigenas'];
                                    ?>
                                </td>
                                <td align="right">
                                    <?php
                                        echo $dadosQuest['qejvlrmecindigenas'] ? $dadosQuest['qejvlrmecindigenas'] : '-';
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td colspan="9" class="SubTituloEsquerdaMenor" style="text-align: left; color: red;"> <b> - <?=$unid_2;?> no censo escolar com atendimento à comunidade Indígenas.</b> </td>
                        <? } ?>
                    </tr>
                    <!--FIM PERGUNTA D-->

                    <!--INÍCIO PERGUNTA E-->
                    <tr>
                        <td class ="SubTituloDireita">E - Pessoas privadas de Liberdade: Anos Iniciais</td>
                        <td>
                            <?php
                                echo $qejprivliberanosiniciais = $dadosQuest['qejprivliberanosiniciais'];
                            ?>
                        </td>
                        <td class ="SubTituloDireitaMenor">Anos Finais:</td>
                        <td>
                            <?php
                                echo $qejprivliberanosfinais = $dadosQuest['qejprivliberanosfinais'];
                            ?>
                        </td>

                        <? if($muncod_estuf == 'M'){ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional:</td>
                                <td>
                                    <?php
                                        echo $qejprivliberqualiprof = $dadosQuest['qejprivliberqualiprof'];
                                    ?>
                                </td>
                        <? }else{ ?>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/fundamental:</td>
                                <td>
                                    <?php
                                        echo $qejprivliberqualiprof_fund = $dadosQuest['qejprivliberqualiprof_fund'];
                                    ?>
                                </td>
                                <td class ="SubTituloDireitaMenor">EJA Integrada a qualificação profissional/médio:</td>
                                <td>
                                    <?php
                                        echo $qejprivliberqualiprof_medi = $dadosQuest['qejprivliberqualiprof_medi'];
                                    ?>
                                </td>
                        <? } ?>

                        <td class ="SubTituloDireitaMenor">Total:</td>
                        <td>
                            <?php
                                echo $total_liberdade = $dadosQuest['total_liberdade'];
                            ?>
                        </td>
                        <td align="right">
                            <?php
                                echo $dadosQuest['qejvlrmecpessoaliberdade'] ? $dadosQuest['qejvlrmecpessoaliberdade'] : '-';
                            ?>
                        </td>
                    </tr>
                    <!--FIM PERGUNTA E-->

                    <tr>
                        <td class ="SubTituloDireita">F - Total de Matrículas do Público Prioritário:</td>
                        <td colspan="<?=$colspan;?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <td>
                            <?php
                                echo $total_prioritario = $dadosQuest['total_prioritario'];
                            ?>
                        </td>
                        <td>
                            <?php
                                echo $dadosQuest['qejvlrmec_total'] ? $dadosQuest['qejvlrmec_total'] : '-';
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="10" class ="SubTituloEsquerda">&nbsp;</td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita">Total Geral:</td>
                        <td colspan="<?=$colspan;?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <td>
                            <?php
                                echo $total_geral = $dadosQuest['total_geral'];
                            ?>
                        </td>
                        <td align="right">
                            <?php
                                echo $dadosQuest['qejvlrmec_total_geral'] ? $dadosQuest['qejvlrmec_total_geral'] : '-';
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class ="SubTituloDireita">Valor Liberado pelo MEC R$:</td>
                        <td colspan="<?=$colspan+1?>" class ="SubTituloDireitaMenor">&nbsp;</td>
                        <td align="right">
                            <?php
                                echo $dadosQuest['qejvlrmec_valor_liberado'] ? $dadosQuest['qejvlrmec_valor_liberado'] : '-';
                            ?>
                        </td>
                    </tr>
                </table>

                <br>
                <!--ACABO-->
                <table align="center" border="1" class="tabela" cellpadding="3" cellspacing="1" style="font-size:8px;" width="100%">
                    <tr style="text-align: center;">
                        <td colspan="4" class ="SubTituloCentro"> Total de matrículas da rede pública <?php echo $descricao_rede; ?> de EJA - Ensino Fundamental / Presencial. </td>
                        <td colspan="8" class ="SubTituloCentro"> Total de matrículas dos Egressos do PBA </td>
                    </tr>
                    <tr>
                        <td colspan="4" style="text-align: center; font-weight: bold;">Ano</td>
                        <td colspan="8" style="text-align: center; font-weight: bold;">Ano</td>
                    </tr>
                    <tr style="text-align: center;">
                        <td style="font-weight: bold;">2010</td>
                        <td style="font-weight: bold;">2011</td>
                        <td style="font-weight: bold;">2012</td>
                        <td style="font-weight: bold;">2013</td>

                        <td colspan="2" style="font-weight: bold;">2010</td>
                        <td colspan="2" style="font-weight: bold;">2011</td>
                        <td colspan="2" style="font-weight: bold;">2012</td>
                        <td colspan="2" style="font-weight: bold;">2013</td>
                    </tr>
                    <tr style="text-align: center;">
                        <td><?php echo $total_matricula['2010'] ? $total_matricula['2010'] : '0'; ?></td>
                        <td><?php echo $total_matricula['2011'] ? $total_matricula['2011'] : '0'; ?></td>
                        <td><?php echo $total_matricula['2012'] ? $total_matricula['2012'] : '0'; ?></td>
                        <td><?php echo $total_matricula['2013'] ? $total_matricula['2013'] : '0'; ?></td>

                        <td>Prefeitura</td>
                        <td>Secretaria</td>
                        <td>Prefeitura</td>
                        <td>Secretaria</td>
                        <td>Prefeitura</td>
                        <td>Secretaria</td>
                        <td>Prefeitura</td>
                        <td>Secretaria</td>
                    </tr>
                    <tr style="text-align: center;">
                        <td colspan="3">&nbsp;</td>
                        <td><?php echo $num_mat[0]['numeromatriculas'] = ( $num_mat[0]['ano'] == '2010' && $num_mat[0]['ent_matricula'] == 'PREF' && $num_mat[0]['numeromatriculas'] != '' ) ? $num_mat[0]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[1]['numeromatriculas'] = ( $num_mat[1]['ano'] == '2010' && $num_mat[1]['ent_matricula'] == 'SECR' && $num_mat[1]['numeromatriculas'] != '' ) ? $num_mat[1]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[2]['numeromatriculas'] = ( $num_mat[2]['ano'] == '2011' && $num_mat[2]['ent_matricula'] == 'PREF' && $num_mat[2]['numeromatriculas'] != '' ) ? $num_mat[2]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[3]['numeromatriculas'] = ( $num_mat[3]['ano'] == '2011' && $num_mat[3]['ent_matricula'] == 'SECR' && $num_mat[3]['numeromatriculas'] != '' ) ? $num_mat[3]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[4]['numeromatriculas'] = ( $num_mat[4]['ano'] == '2012' && $num_mat[4]['ent_matricula'] == 'PREF' && $num_mat[4]['numeromatriculas'] != '' ) ? $num_mat[4]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[5]['numeromatriculas'] = ( $num_mat[5]['ano'] == '2012' && $num_mat[5]['ent_matricula'] == 'SECR' && $num_mat[5]['numeromatriculas'] != '' ) ? $num_mat[5]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[6]['numeromatriculas'] = ( $num_mat[6]['ano'] == '2013' && $num_mat[6]['ent_matricula'] == 'PREF' && $num_mat[6]['numeromatriculas'] != '' ) ? $num_mat[6]['numeromatriculas'] : '0'; ?></td>
                        <td><?php echo $num_mat[7]['numeromatriculas'] = ( $num_mat[7]['ano'] == '2013' && $num_mat[7]['ent_matricula'] == 'SECR' && $num_mat[7]['numeromatriculas'] != '' ) ? $num_mat[7]['numeromatriculas'] : '0'; ?></td>
                    </tr>
                </table>

<?php

            }

            $existe_parecer = existeParecer();

            if( $existe_parecer == 'S' ){
?>
                <br>

                <table align="center"border="1" class="tabela" cellpadding="3" cellspacing="1" style="font-size:11px;" width="100%">
                    <tr>
                        <td colspan="5" class ="SubTituloCentro" style="text-align: left;">Parecer</td>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: justify; height: auto;">
                            <?php
                                global $db;
                                $sql = "
                                    SELECT pejresposta FROM eja.parecereja WHERE pejstatus = 'A' AND adpid = {$_SESSION['par']['adpid']} AND pejsituacao = 't'
                                ";
                                $pejresposta = $db->pegaUm($sql);
                                echo $pejresposta;
                            ?>
                        </td>
                    </tr>
                </table>
<?php
                $sql = "
                    SELECT  u.usunome,
                            replace(to_char(cast(p.usucpf as bigint), '000:000:000-00'), ':', '.') as usucpf,
                            to_char(p.adpdataresposta, 'DD/MM/YYYY') AS adpdataresposta
                    FROM par.pfadesaoprograma p
                    JOIN seguranca.usuario AS u ON u.usucpf = p.usucpf
                    WHERE p.adpid = {$_SESSION['par']['adpid']}
                ";
                $dados = $db->pegaLinha($sql);
                echo "<br>";
                echo "________________________________________________________________________";
                echo "<br>";
                echo "<b style=\"font-size:11px;\">Termo Aceito por: {$dados['usunome']} - CPF: {$dados['usucpf']} em {$dados['adpdataresposta']}</b>";
            }
        }
    }
?>