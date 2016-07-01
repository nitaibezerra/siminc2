<?PHP
    # - atualizaComboMunicipio
    function atualizaComboMunicipio($dados){
        global $db;

        if( $_SESSION['par']['muncod'] != '' ){
            $estuf = $dados['estuf'];

            $muncod = $dadosInstitucional['muncod'];

            $sql = "
                SELECT  muncod AS codigo,
                        mundescricao AS descricao
                FROM territorios.municipio

                WHERE estuf =  '{$estuf}'

                ORDER BY descricao
            ";
            $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 250, 'N', 'muncod', '', $muncod);
            die();
        }
    }

    # - anexarDocumentos: TELA CADASTRO DE CESSÃO - ANEXAR DOCUMENTOS.
    function anexarDocumentos($dados, $files) {
        global $db;

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $adpid          = $_SESSION['par']['adpid'];
        $usucpf         = $_SESSION['usucpf'];
        $tpaid          = $dados['tpaid'];
        $apedtinclusao  = "'".gmdate('Y-m-d')."'";

        $campos = array(
            "adpid"         => $adpid,
            "usucpf"        => "'".$usucpf."'",
            "tpaid"         => $tpaid,
            "apestatus"     => "'A'",
            "apedtinclusao" => $apedtinclusao
        );

        $file = new FilesSimec("arquivoeja", $campos, "eja");

        if ( $files ) {
            $arquivoSalvo = $file->setUpload("EJA - PRONATEC - Adesão ao Programa", "arquivo");
            if ($arquivoSalvo) {
                $db->sucesso('principal/programas/feirao_programas/eja_pronatec/eja_pronatec_documentos', '&acao=A');
            }else{
                $db->sucesso('principal/cessao_prorrogacao/cad_dados_documentos', '&acao=A');
            }
        }
        exit;
    }

    function buscarEndereceCEP( $dados ){
        global $db;

        $cep = str_replace('-', '', $dados['cep']);

        $sql = "
            SELECT * FROM cep.v_endereco2 WHERE cep = '{$cep}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            $dados["logradouro"]    = iconv("ISO-8859-1", "UTF-8", $dados["logradouro"]);
            $dados["bairro"]        = iconv("ISO-8859-1", "UTF-8", $dados["bairro"]);
            $dados["muncod"]        = iconv("ISO-8859-1", "UTF-8", $dados["muncod"]);
            echo simec_json_encode( $dados );
        }else{
            $dados["logradouro"]    = "";
            $dados["bairro"]        = "";
            $dados["estado"]        = "";
            $dados["muncod"]        = "";
            echo simec_json_encode( $dados );
        }
        die;
    }


    # - bucarMunicipioBloqueado: BUSCAR MUNICIPIO BLOQUEADO - USADO PELA TELA DE INFORMATIVO PARA ADESÃO.
    function bucarMunicipioBloqueado(){
        global $db;

        $sql = "
            SELECT codigoibge AS muncod FROM eja.ejacruzamento WHERE codigoibge = '{$_SESSION['par']['muncod']}';
        ";
        $muncod = $db->pegaUm($sql);

        if( $muncod == '' ){
            $bloqueado = "S";
        }else{
            $bloqueado = "N";
        }
        return $bloqueado;
    }

    # - buscaDadosInstitucional: BUSCA OS DADOS DA TELA DE INSTITUCIONAL.
    function buscaDadosInstitucional( $adpid ){
        global $db;

        $sql = "
            SELECT  epiid,
                    estuf,
                    muncod,
                    adpid,
                    TRIM( epidsc ) AS epidsc,
                    TRIM( replace(to_char(cast(epicnpj as bigint), '00:000:000/0000-00'), ':', '.') ) AS epicnpj,
                    TRIM(epitelefonenumero) AS epitelefonenumero,
                    TRIM( epiemail ) AS epiemail,

                    TRIM( replace(to_char(cast(epicep as bigint), '00000-000'), ':', '.') ) AS epicep,

                    TRIM( epilogradouro ) AS epilogradouro,

                    TRIM( epicomplemento ) AS epicomplemento,
                    TRIM( epibairro ) AS epibairro

            FROM eja.ejapronatecinstitucional

            WHERE adpid = {$adpid}
        ";
        $dados = $db->pegaLinha($sql);

        return $dados;
    }

    # - buscaDadosSupervisoeDemandas: BUSCA OS DADOS DA TELA DE SUPERVIDOR DE DEMANDAS.
    function buscaDadosInstitucionais( $adpid ){
        global $db;

        $sql = "
            SELECT  epidsc,
                    epicnpj,
                    epiemail,
                    epicep,
                    epilogradouro,
                    epibairro
            FROM eja.ejapronatecinstitucional

            WHERE adpid = {$adpid}
        ";
        $dados = $db->pegaLinha($sql);

        return $dados;
    }

    # - buscaDadosSupervisoeDemandas: BUSCA OS DADOS DA TELA DE SUPERVIDOR DE DEMANDAS.
    function buscaDadosSupervisoeDemandas( $adpid ){
        global $db;

        $sql = "
            SELECT  epsid,
                    adpid,
                    muncod,
                    TRIM( epsnome ) AS epsnome,
                    TRIM( replace(to_char(cast(epscpf as bigint), '000:000:000-00'), ':', '.') ) AS epscpf,
                    TRIM( epsmatricula ) AS epsmatricula,
                    TRIM( epsrg ) AS epsrg,
                    TRIM( epsrgorgaoexp ) AS epsrgorgaoexp,
                    TRIM( estufrgorgaoexp ) AS estufrgorgaoexp,
                    TO_CHAR( epsdtexprg, 'DD/MM/YYYY') AS epsdtexprg,
                    TRIM( epstelefone ) AS epstelefone,
                    TRIM( epscelular ) AS epscelular,
                    TRIM( epsemailinstitucional ) AS epsemailinstitucional,
                    TRIM( epsemailparticular ) AS epsemailparticular,
                    TO_CHAR( epsdtinicio, 'DD/MM/YYYY') AS epsdtinicio
            FROM eja.ejapronatecsupervisor

            WHERE adpid = {$adpid}
        ";
        $dados = $db->pegaLinha($sql);

        return $dados;
    }

    # - buscaDadosQuestPronatec: BUSCA OS DADOS DA TELA SUPERVIDOR DE DEMANDAS.
    function buscaDadosQuestPronatec( $inuid ){
        global $db;

        $sql = "
            SELECT  qepid,
                    qeppnpensinofund,
                    qepppbraalfensfund,
                    qeppppopcampoensfund,
                    qepppquilombolaensfund,
                    qepppindigenasensfund,
                    qepppprovliberensfund,
                    qepppmedsocioensfund,
                    qepppmatreciclensfund,
                    qepppsitruaensfund,
                    qeppppescadoresensfund,

                    (
                        qeppnpensinofund+qepppbraalfensfund+qeppppopcampoensfund+qepppquilombolaensfund+
                        qepppindigenasensfund+qepppprovliberensfund+qepppmedsocioensfund+qepppmatreciclensfund+
                        qepppsitruaensfund+qeppppescadoresensfund
                    ) AS total_geral_fundamental,

                    (
                        qepppbraalfensmedio+qeppppopcampoensmedio+qepppquilombolaensmedio+qepppindigenasensmedio+
                        qepppprovliberensmedio+qepppmedsocioensmedio+qepppmatreciclensmedio+qepppsitruaensmedio+
                        qeppppescadoresensmedio
                    ) AS total_geral_medio,

                    qepquestao01,
                    qepquestao01qtd,

                    qepppbraalfensmedio,
                    qeppppopcampoensmedio,
                    qepppquilombolaensmedio,
                    qepppindigenasensmedio,
                    qepppprovliberensmedio,
                    qepppmedsocioensmedio,
                    qepppmatreciclensmedio,
                    qepppsitruaensmedio,
                    qeppppescadoresensmedio,
                    qepejatecnicointegrado,
                    qepejatecnicoconcomitante,
                    qepejaficmedio,
                    qepejaficfundamental

            FROM eja.questionarioejapronatec
            WHERE inuid = {$inuid}
        ";
        $dados = $db->pegaLinha($sql);

        return $dados;
    }

    # - continuaAdesaoPronatec: DA CONTINUIDADE A ADESÃO AO PROGRAMA EJA PRONARTEC - USADO NA TELA INFORMATIVO.
    function continuaAdesaoPronatec(){
        $_SESSION['continuaAdesaoPronatec'] = 'S';
        header("Location:par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A");
        exit();
    }

    # - downloadDocAnexo: TELA ANEXAR DOCUMENTO - FAZ O DOWNLOAD DOS DOCUMENTOS ANEZADOS.
    function donwloadDocAnexo( $dados, $file = '' ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ($arqid){
            $file = new FilesSimec("arquivoeja", $campos, "eja");
            $file->getDownloadArquivo( $arqid );
        }
    }

    # - excluirDocAnexo: TELA ANEXAR DOCUMENTO - DELETA OS DOCUMENTOS ANEXADOS.
   function excluirDocAnexo( $dados ) {
       global $db;

       $arqid = $dados['arqid'];

       include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

       if ($arqid != '') {
           $sql = " UPDATE eja.arquivoeja SET apestatus = 'I' WHERE arqid = {$arqid} ";
       }

       if( $db->executar($sql) ){
           $file = new FilesSimec("arquivoeja", $campos, "eja");
           $file->excluiArquivoFisico( $arqid );

           $db->commit();
           $db->sucesso('principal/programas/feirao_programas/eja_pronatec/eja_pronatec_documentos', '&acao=A');
       }
   }


    # - permissaoPrevisaoOferta: VERIFICA QUAL O TIPO DE PERMISSÃO DO USUÁRIO - USADO NAS TELAS.
    function permissaoPrevisaoOferta(){
        global $db;

        $perfil = pegaArrayPerfil( $_SESSION['usucpf'] );

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        $prog   = pegarProgramaDisponivel();

        $existe_parecer = existeParecer();

        if( in_array(PROG_PAR_EJA_PRONATEC, $prog) ){

            if( $estado == WF_EJA_PRONATEC_EM_PREENCHIMENTO_UNIDADE ){

                $habilitado['TEXT']  = 'S';
                $habilitado['BOTAO'] = 'S';
                $habilitado['RADIO'] = '';

            }elseif( $estado == WF_EJA_PRONATEC_EM_ANALISE_MEC ){

                if( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ANALISTA_EJA, $perfil) ){
                    $habilitado['BOTAO'] = 'S';
                    $habilitado['TEXT']  = 'N';
                    $habilitado['RADIO'] = 'disabled="disabled"';
                }else{
                    $habilitado['BOTAO'] = 'N';
                    $habilitado['TEXT']  = 'N';
                }

            }else{
                $habilitado['BOTAO'] = 'N';
                $habilitado['TEXT']  = 'N';
                $habilitado['RADIO'] = 'disabled="disabled"';
            }
/*
            if($estado != WF_EJA_PRONATEC_APROVADO){
                if( $existe_parecer == 'S' || $estado != WF_EJA_PRONATEC_EM_PREENCHIMENTO_UNIDADE ){

                    if( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ANALISTA_EJA, $perfil) ){
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
*/
        }else{
            //$habilitado['ANALISE']  = 'N';
            $habilitado['BOTAO']    = 'N';
            $habilitado['TEXT']     = 'N';
            $habilitado['RADIO']    = 'disabled="disabled"';
            //$habilitado['PARECER_HAB'] = 'N';
            //$habilitado['BOTAO_PAREC'] = 'disabled="disabled"';
            //$habilitado['DESCRICAO_P'] = 'N';
            //$habilitado['RADIO_PAREC'] = 'disabled="disabled"';
        }

        return $habilitado;
    }

/*
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
*/

    # - salvarEJA_Institucional: SALVA DADOS DA ISNTITUIÇÃO REPRESENTANTE DA "EDUCAÇÃO E CULTURA" REFERENTE A ESFERA.
    function salvarEJA_Institucional( $dados ){
        global $db;

        $epiid  = $dados['epiid'];

        $adpid  = $_SESSION['par']['adpid'];
        //$muncod = $_SESSION['par']['muncod'];

        $epidsc             = trim($dados['epidsc']);
        $epicnpj            = str_replace( '.', '', str_replace( '-', '', str_replace( '/', '', $dados['epicnpj']) ) );
        $epitelefonenumero  = trim($dados['epitelefonenumero']);
        $epiemail           = trim($dados['epiemail']);
        $epicep             = trim( str_replace('-', '', $dados['epicep']) );
        $epilogradouro      = trim($dados['epilogradouro']);
        $epicomplemento     = trim( $dados['epicomplemento'] );
        $epibairro          = trim($dados['epibairro']);
        $estuf              = trim($dados['estuf']) ? "'".trim($dados['estuf'])."'" : "NULL";
        $muncod             = trim($dados['muncod']) ? "'".trim($dados['muncod'])."'" : "NULL";

        if( $epiid == '' ){
            $sql = "
                INSERT INTO eja.ejapronatecinstitucional(
                        estuf, muncod, adpid, epidsc, epicnpj, epitelefoneddd, epitelefonenumero, epiemail, epicep, epilogradouro, epicomplemento, epibairro
                    )VALUES (
                        {$estuf}, {$muncod}, {$adpid}, '{$epidsc}', '{$epicnpj}', '{$epitelefoneddd}', '{$epitelefonenumero}', '{$epiemail}',
                        replace(replace('{$epicep}','-',''),'.',''), '{$epilogradouro}', '{$epicomplemento}', '{$epibairro}'
                ) RETURNING epiid;
            ";
        } else {
            $sql = "
                UPDATE eja.ejapronatecinstitucional
                    SET estuf               = {$estuf},
                        muncod              = {$muncod},
                        adpid               = {$adpid},
                        epidsc              = '{$epidsc}',
                        epicnpj             = '{$epicnpj}',
                        epitelefoneddd      = '{$epitelefoneddd}',
                        epitelefonenumero   = '{$epitelefonenumero}',
                        epiemail            = '{$epiemail}',
                        epicep              = replace(replace('{$epicep}','-',''),'.',''),
                        epilogradouro       = '{$epilogradouro}',
                        epicomplemento      = '{$epicomplemento}',
                        epibairro           = '{$epibairro}'
                WHERE epiid = {$epiid} RETURNING epiid;
            ";
        }
        $epiid = $db->pegaUm($sql);

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        if($epiid > 0){
            $db->commit();
            $db->sucesso('principal/programas/feirao_programas/eja_pronatec/eja_pronatec_institucional', '', 'Operação realizada com sucesso!');
        } else {
            $db->insucesso('Não foi possível gravar o registro!', '', 'principal/programas/feirao_programas/eja_pronatec/eja_pronatec_institucional&acao=A');
        }
    }

    # - salvarEJA_PRONATEC: SALVA DADOS DO SUPERVISOR DE DEMANDAS - TELA SERVIDOR DE DEMANDAS.
    function salvarSupervisorEJA_PRONATEC( $dados ){
        global $db;

        $epsid  = $dados['epsid'];

        $adpid  = $_SESSION['par']['adpid'];
        $muncod = $_SESSION['par']['muncod'] ? "'".$_SESSION['par']['muncod']."'" : "NULL";

        $epsnome                = trim($dados['epsnome']);
        $epscpf                 = str_replace('.', '', str_replace('-', '', $dados['epscpf']) );
        $epsmatricula           = trim($dados['epsmatricula']);

        $epsrg                  = $dados['epsrg']           ? trim($dados['epsrg']) : '';
        $epsrgorgaoexp          = $dados['epsrgorgaoexp']   ? trim($dados['epsrgorgaoexp']) : '';
        $estufrgorgaoexp        = $dados['estufrgorgaoexp'] ? "'".trim($dados['estufrgorgaoexp'])."'" : 'NULL';
        $epsdtexprg             = $dados['epsdtexprg']      ? "'".formata_data_sql( $dados['epsdtexprg'] )."'" : 'NULL';

        $epstelefone            = trim($dados['epstelefone']);
        $epscelular             = trim($dados['epscelular']);
        $epsemailinstitucional  = trim($dados['epsemailinstitucional']);
        $epsemailparticular     = trim($dados['epsemailparticular']);
        $epsdtinicio            = formata_data_sql( $dados['epsdtinicio'] );

        if( $epsid == '' ){
            $sql = "
                INSERT INTO eja.ejapronatecsupervisor(
                        adpid,
                        muncod,
                        epsnome,
                        epscpf,
                        epsmatricula,
                        epsrg,
                        epsrgorgaoexp,
                        estufrgorgaoexp,
                        epsdtexprg,
                        epstelefone,
                        epscelular,
                        epsemailinstitucional,
                        epsemailparticular,
                        epsdtinicio
                ) VALUES (
                        {$adpid},
                        {$muncod},
                        '{$epsnome}',
                        '{$epscpf}',
                        '{$epsmatricula}',
                        '{$epsrg}',
                        '{$epsrgorgaoexp}',
                        {$estufrgorgaoexp},
                        {$epsdtexprg},
                        '{$epstelefone}',
                        '{$epscelular}',
                        '{$epsemailinstitucional}',
                        '{$epsemailparticular}',
                        '{$epsdtinicio}'
                ) RETURNING epsid;
            ";
        } else {
            $sql = "
                UPDATE eja.ejapronatecsupervisor
                    SET adpid                   = {$adpid},
                        muncod                  = {$muncod},
                        epsnome                 = '{$epsnome}',
                        epscpf                  = '{$epscpf}',
                        epsmatricula            = '{$epsmatricula}',
                        epsrg                   = '{$epsrg}',
                        epsrgorgaoexp           = '{$epsrgorgaoexp}',
                        estufrgorgaoexp         = {$estufrgorgaoexp},
                        epsdtexprg              = {$epsdtexprg},
                        epstelefone             = '{$epstelefone}',
                        epscelular              = '{$epscelular}',
                        epsemailinstitucional   = '{$epsemailinstitucional}',
                        epsemailparticular      = '{$epsemailparticular}',
                        epsdtinicio             = '{$epsdtinicio}'
                WHERE epsid = {$epsid} RETURNING epsid;
            ";
        }
        $epsid = $db->pegaUm($sql);

        $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
        $estado = pgPegarEstadoAtual($docid);

        if($epsid > 0){
            $db->commit();
            $db->sucesso('principal/programas/feirao_programas/eja_pronatec/eja_pronatec_questionario', '', 'Operação realizada com sucesso!');
        } else {
            $db->insucesso('Não foi possível gravar o registro!', '', 'principal/programas/feirao_programas/eja_pronatec/eja_pronatec_supervisor&acao=A');
        }
    }

    # - salvarQuestEJA_PRONATEC: SALVA DADOS DO QUESTIONARIO DE ESTIMATIVAS DE MATRICULAS - USADO PELA TELA ESTIMATIVAS DE MATRICULAS.
    function salvarQuestEJA_PRONATEC( $dados ){
        global $db;

        $qepid  = $dados['qepid'];

        $inuid  = $_SESSION['par']['inuid'];
        $usucpf = $_SESSION['usucpf'];

        $qeppnpensinofund       = $dados['qeppnpensinofund'] ? $dados['qeppnpensinofund'] : 0;
        $qepppbraalfensfund     = $dados['qepppbraalfensfund'] ? $dados['qepppbraalfensfund'] : 0;
        $qeppppopcampoensfund   = $dados['qeppppopcampoensfund'] ? $dados['qeppppopcampoensfund'] : 0;
        $qepppquilombolaensfund = $dados['qepppquilombolaensfund'] ? $dados['qepppquilombolaensfund'] : 0;
        $qepppindigenasensfund  = $dados['qepppindigenasensfund'] ? $dados['qepppindigenasensfund'] : 0;
        $qepppprovliberensfund  = $dados['qepppprovliberensfund'] ? $dados['qepppprovliberensfund'] : 0;
        $qepppmedsocioensfund   = $dados['qepppmedsocioensfund'] ? $dados['qepppmedsocioensfund'] : 0;
        $qepppmatreciclensfund  = $dados['qepppmatreciclensfund'] ? $dados['qepppmatreciclensfund'] : 0;
        $qepppsitruaensfund     = $dados['qepppsitruaensfund'] ? $dados['qepppsitruaensfund'] : 0;
        $qeppppescadoresensfund = $dados['qeppppescadoresensfund'] ? $dados['qeppppescadoresensfund'] : 0;
        $qepquestao01           = $dados['qepquestao01'] == 'S' ? 't' : 'f';
        $qepquestao01qtd        = $dados['qepquestao01qtd'] ? $dados['qepquestao01qtd'] : 0;

        $qepejatecnicointegrado     = $dados['qepejatecnicointegrado'] == 'S' ? 't' : 'f';
        $qepejatecnicoconcomitante  = $dados['qepejatecnicoconcomitante'] == 'S' ? 't' : 'f';
        $qepejaficmedio             = $dados['qepejaficmedio'] == 'S' ? 't' : 'f';
        $qepejaficfundamental       = $dados['qepejaficfundamental'] == 'S' ? 't' : 'f';

        $qepppbraalfensmedio        = $dados['qepppbraalfensmedio'] ? $dados['qepppbraalfensmedio'] : 0;
        $qeppppopcampoensmedio      = $dados['qeppppopcampoensmedio'] ? $dados['qeppppopcampoensmedio'] : 0;
        $qepppquilombolaensmedio    = $dados['qepppquilombolaensmedio'] ? $dados['qepppquilombolaensmedio'] : 0;
        $qepppindigenasensmedio     = $dados['qepppindigenasensmedio'] ? $dados['qepppindigenasensmedio'] : 0;
        $qepppprovliberensmedio     = $dados['qepppprovliberensmedio'] ? $dados['qepppprovliberensmedio'] : 0;
        $qepppmedsocioensmedio      = $dados['qepppmedsocioensmedio'] ? $dados['qepppmedsocioensmedio'] : 0;
        $qepppmatreciclensmedio     = $dados['qepppmatreciclensmedio'] ? $dados['qepppmatreciclensmedio'] : 0;
        $qepppsitruaensmedio        = $dados['qepppsitruaensmedio'] ? $dados['qepppsitruaensmedio'] : 0;
        $qeppppescadoresensmedio    = $dados['qeppppescadoresensmedio'] ? $dados['qeppppescadoresensmedio'] : 0;



        if( $qepid == '' ){
            $sql = "
                INSERT INTO eja.questionarioejapronatec(
                        inuid,
                        usucpf,
                        qeppnpensinofund,
                        qepppbraalfensfund,
                        qeppppopcampoensfund,
                        qepppquilombolaensfund,
                        qepppindigenasensfund,
                        qepppprovliberensfund,
                        qepppmedsocioensfund,
                        qepppmatreciclensfund,
                        qepppsitruaensfund,
                        qeppppescadoresensfund,
                        qepquestao01,
                        qepquestao01qtd,

                        qepppbraalfensmedio,
                        qeppppopcampoensmedio,
                        qepppquilombolaensmedio,
                        qepppindigenasensmedio,
                        qepppprovliberensmedio,
                        qepppmedsocioensmedio,
                        qepppmatreciclensmedio,
                        qepppsitruaensmedio,
                        qeppppescadoresensmedio,
                        qepejatecnicointegrado,
                        qepejatecnicoconcomitante,
                        qepejaficmedio,
                        qepejaficfundamental

                    ) VALUES (
                        {$inuid},
                        '{$usucpf}',
                        {$qeppnpensinofund},
                        {$qepppbraalfensfund},
                        {$qeppppopcampoensfund},
                        {$qepppquilombolaensfund},
                        {$qepppindigenasensfund},
                        {$qepppprovliberensfund},
                        {$qepppmedsocioensfund},
                        {$qepppmatreciclensfund},
                        {$qepppsitruaensfund},
                        {$qeppppescadoresensfund},
                        '{$qepquestao01}',
                        {$qepquestao01qtd},

                        '{$qepppbraalfensmedio}',
                        '{$qeppppopcampoensmedio}',
                        '{$qepppquilombolaensmedio}',
                        '{$qepppindigenasensmedio}',
                        '{$qepppprovliberensmedio}',
                        '{$qepppmedsocioensmedio}',
                        '{$qepppmatreciclensmedio}',
                        '{$qepppsitruaensmedio}',
                        '{$qeppppescadoresensmedio}',
                        '{$qepejatecnicointegrado}',
                        '{$qepejatecnicoconcomitante}',
                        '{$qepejaficmedio}',
                        '{$qepejaficfundamental}'
                ) RETURNING qepid;
        ";
    } else {
        $sql = "
            UPDATE eja.questionarioejapronatec
                SET usucpf                  = '{$usucpf}',
                    qeppnpensinofund        = {$qeppnpensinofund},
                    qepppbraalfensfund      = {$qepppbraalfensfund},
                    qeppppopcampoensfund    = {$qeppppopcampoensfund},
                    qepppquilombolaensfund  = {$qepppquilombolaensfund},
                    qepppindigenasensfund   = {$qepppindigenasensfund},
                    qepppprovliberensfund   = {$qepppprovliberensfund},
                    qepppmedsocioensfund    = {$qepppmedsocioensfund},
                    qepppmatreciclensfund   = {$qepppmatreciclensfund},
                    qepppsitruaensfund      = {$qepppsitruaensfund},
                    qeppppescadoresensfund  = {$qeppppescadoresensfund},
                    qepquestao01            = '{$qepquestao01}',
                    qepquestao01qtd         = {$qepquestao01qtd},

                    qepppbraalfensmedio         = '{$qepppbraalfensmedio}',
                    qeppppopcampoensmedio       = '{$qeppppopcampoensmedio}' ,
                    qepppquilombolaensmedio     = '{$qepppquilombolaensmedio}',
                    qepppindigenasensmedio      = '{$qepppindigenasensmedio}',
                    qepppprovliberensmedio      = '{$qepppprovliberensmedio}',
                    qepppmedsocioensmedio       = '{$qepppmedsocioensmedio}',
                    qepppmatreciclensmedio      = '{$qepppmatreciclensmedio}',
                    qepppsitruaensmedio         = '{$qepppsitruaensmedio}',
                    qeppppescadoresensmedio     = '{$qeppppescadoresensmedio}',

                    qepejatecnicointegrado      = '{$qepejatecnicointegrado}',
                    qepejatecnicoconcomitante   = '{$qepejatecnicoconcomitante}',
                    qepejaficmedio              = '{$qepejaficmedio}',
                    qepejaficfundamental        = '{$qepejaficfundamental}'

              WHERE qepid = {$qepid} AND inuid = {$inuid} RETURNING qepid;
        ";
    }
    $qepid = $db->pegaUm($sql);

    $docid  = pgCriarDocumento( $_SESSION['par']['adpid'] );
    $estado = pgPegarEstadoAtual($docid);

    if($qepid > 0){
        $db->commit();
        $db->sucesso('principal/programas/feirao_programas/eja_pronatec/eja_pronatec_documentos', '', 'Operação realizada com sucesso!');
    } else {
        $db->insucesso('Não foi possível gravar o registro!', '', 'principal/programas/feirao_programas/eja_pronatec/eja_pronatec_questionario&acao=A');
    }
}

# - programaDataHabil: VERIFICA A DATA HABIL PARA A ADESÃO AO PROGRAMA.
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

# - verificaPermisEnvioAnalise: VERIFICA O PREENCHIMENTO DE 2 TELAS, CASO ESTEJA PREENCHIDO HABILITA O WORK FLOW.
function verificaPermisEnvioAnalise(){
    global $db;

    $adpid = $_SESSION['par']['adpid'];
    $inuid = $_SESSION['par']['inuid'];

    $sql_1 = "
        SELECT epsid FROM eja.ejapronatecsupervisor  WHERE adpid = {$adpid}
    ";
    $epsid = $db->pegaUm($sql_1);

    $sql_2 = "
        SELECT qepid FROM eja.questionarioejapronatec WHERE inuid = {$inuid}
    ";
    $qepid = $db->pegaUm($sql_2);

    if($epsid > 0 && $qepid > 0){
        return true;
    }else{
        return false;
    }
}

# - verificaRegrasAlfabetizado: BUSCA SE O MUNICIPIO ESTA NO SENSO 2013 COM ESCOLA PROG. BRASIL ALFABETIZADO. SE SIM VISUALIZA A TR DA PERGUNTA "A" DO QUESTIONARIO - USADO NA TELA DE QUESTIONARIO.
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

# - verificaRegrasIndiginas: BUSCA SE O MUNICIPIO ESTA NO SENSO 2013 COM ESCOLA INDIGINA. SE SIM VISUALIZA A TR DA PERGUNTA "D" DO QUESTIONARIO - USADO NA TELA DE QUESTIONARIO.
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

# - dadosImpressaoTermoAdsaoPronatec: MONTA AS TABELAS DOS DADOS DO SERVIDOR E DO QUESTIONARIO - USADO NA TELA DE ADSÃO.
function dadosImpressaoTermoAdsaoPronatec(){
    $dadosEjaInstituc = buscaDadosInstitucionais( $_SESSION['par']['adpid'] );
    $dadosEjaPronatec = buscaDadosSupervisoeDemandas( $_SESSION['par']['adpid'] );
    $dadosQuestPronatec = buscaDadosQuestPronatec( $_SESSION['par']['inuid'] );

    $docid = pgCriarDocumento($_SESSION['par']['adpid']);
    $estado = pgPegarEstadoAtual($docid);

    if( $estado != WF_EJA_PRONATEC_EM_PREENCHIMENTO_UNIDADE ){
?>
        <br>
        <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1" style="font-size: 8px; width: 98%;">
            <tr>
                <td colspan="6" class="SubTituloCentro" style="text-align: center;">
                    DADOS INSTITUCIONAIS
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" width="16%" > Prefeitura Municipal: </td>
                <td width="16%">
                   <?php
                        echo $epsnome = $dadosEjaInstituc['epidsc'];
                    ?>
                </td>
                <td class ="SubTituloDireita" width="16%"> CNPJ: </td>
                <td width="16%">
                   <?php
                        echo $epscpf = $dadosEjaInstituc['epicnpj'];
                    ?>
                </td>
                <td class ="SubTituloDireita" width="16%"> E-mail: </td>
                <td width="16%">
                   <?php
                        echo $epsmatricula = $dadosEjaInstituc['epiemail'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> CEP: </td>
                <td>
                   <?php
                        echo $epsrg = $dadosEjaInstituc['epicep'];
                    ?>
                </td>
                <td class ="SubTituloDireita"> Logradouro: </td>
                <td>
                   <?php
                        echo $epsrgorgaoexp = $dadosEjaInstituc['epilogradouro'];
                    ?>
                </td>
                <td class ="SubTituloDireita"> Bairro: </td>
                <td>
                   <?php
                        echo $epsemailinstitucional = $dadosEjaInstituc['epibairro'];
                    ?>
                </td>
            </tr>
        </table>
        <br>
        <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1" style="font-size: 8px; width: 98%;">
            <tr>
                <td colspan="6" class="SubTituloCentro" style="text-align: center;">
                    DADOS SERVIDOR DE DEMANDAS
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" width="16%" > Nome: </td>
                <td width="16%">
                   <?php
                        echo $epsnome = $dadosEjaPronatec['epsnome'];
                    ?>
                </td>
                <td class ="SubTituloDireita" width="16%"> CPF: </td>
                <td width="16%">
                   <?php
                        echo $epscpf = $dadosEjaPronatec['epscpf'];
                    ?>
                </td>
                <td class ="SubTituloDireita" width="16%"> Matrícula SIAPE: </td>
                <td width="16%">
                   <?php
                        echo $epsmatricula = $dadosEjaPronatec['epsmatricula'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> RG: </td>
                <td>
                   <?php
                        echo $epsrg = $dadosEjaPronatec['epsrg'];
                    ?>
                </td>
                <td class ="SubTituloDireita"> Órgão Exp. RG: </td>
                <td>
                   <?php
                        echo $epsrgorgaoexp = $dadosEjaPronatec['epsrgorgaoexp'];
                    ?>
                </td>
                <td class ="SubTituloDireita"> E-mail iInsitucional: </td>
                <td>
                   <?php
                        echo $epsemailinstitucional = $dadosEjaPronatec['epsemailinstitucional'];
                    ?>
                </td>
            </tr>
        </table>
        <br>
        <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="2" style="font-size: 8px; width: 98%;">
            <tr>
                <td class="SubTituloCentro" colspan="5" style="text-align: center;"> ESTIMATIVA DE MATÍCULAS </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerdaMenor" colspan="5">PUBLÍCO NÃO PRIORITÁRIO</td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" width="45%" > Publíco não Prioritário: </td>
                <td width="10%"> &nbsp; </td>
                <td width="45%" colspan="3">
                   <?php
                        echo $qeppnpensinofund = $dadosQuestPronatec['qeppnpensinofund'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerdaMenor" colspan="5">PUBLÍCO PRIORITÁRIO</td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> A - Atendidos no Programa Brasil Alfabetizado: </td>
                <td class ="SubTituloDireita" rowspan="9">Ensino Fundamental</td>
                <td width="15%">
                   <?php
                        echo $qepppbraalfensfund = $dadosQuestPronatec['qepppbraalfensfund'];
                    ?>
                </td>
                <td width="10%" class ="SubTituloDireita" rowspan="9">Ensino Médio</td>
            <td width="40%">
               <?php
                    echo $qepppbraalfensmedio = $dadosQuestPronatec['qepppbraalfensmedio'];
                ?>
            </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> B - Populações do Campo: </td>
                <td>
                   <?php
                        echo $qeppppopcampoensfund = $dadosQuestPronatec['qeppppopcampoensfund'];
                    ?>
                </td>
                <td width="40%">
               <?php
                    echo $qeppppopcampoensmedio = $dadosQuestPronatec['qeppppopcampoensmedio'];
                ?>
            </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> C - Quilombolas: </td>
                <td>
                   <?php
                        echo $qepppquilombolaensfund = $dadosQuestPronatec['qepppquilombolaensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                        echo $qepppquilombolaensmedio = $dadosQuestPronatec['qepppquilombolaensmedio'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> D - Indiginas: </td>
                <td>
                   <?php
                        echo $qepppindigenasensfund = $dadosQuestPronatec['qepppindigenasensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                        echo $qepppindigenasensmedio = $dadosQuestPronatec['qepppindigenasensmedio'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> E - Pessoas que cumprem pena em privação de liberdade: </td>
                <td>
                    <?php
                        echo $qepppprovliberensfund = $dadosQuestPronatec['qepppprovliberensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                         echo $qepppprovliberensmedio = $dadosQuestPronatec['qepppprovliberensmedio'];
                     ?>
                 </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> F - Jovem em Cumprimento de Medidas Socioeducativas: </td>
                <td>
                   <?php
                        echo $qepppmedsocioensfund = $dadosQuestPronatec['qepppmedsocioensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                         echo $qepppmedsocioensmedio = $dadosQuestPronatec['qepppmedsocioensmedio'];
                     ?>
                 </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> G - Catadores de Materiais Recicláveis: </td>
                <td>
                   <?php
                        echo $qepppmatreciclensfund = $dadosQuestPronatec['qepppmatreciclensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                         echo $qepppmatreciclensmedio = $dadosQuestPronatec['qepppmatreciclensmedio'];
                     ?>
                 </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> H - Populaçao em situação de Rua: </td>
                <td>
                   <?php
                        echo $qepppsitruaensfund = $dadosQuestPronatec['qepppsitruaensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                         echo $qepppsitruaensmedio = $dadosQuestPronatec['qepppsitruaensmedio'];
                     ?>
                 </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> I - Pescadores:</td>
                <td>
                   <?php
                        echo $qeppppescadoresensfund = $dadosQuestPronatec['qeppppescadoresensfund'];
                    ?>
                </td>
                <td width="40%">
                    <?php
                         echo $qeppppescadoresensmedio = $dadosQuestPronatec['qeppppescadoresensmedio'];
                     ?>
                 </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Total Geral: </td>
                <td> &nbsp;</td>
                <td>
                   <?php
                        echo $total_geral_fundamental = $dadosQuestPronatec['total_geral_fundamental'];
                    ?>
                </td>
                <td> &nbsp;</td>
                <td>
                   <?php
                        echo $total_geral_medio = $dadosQuestPronatec['total_geral_medio'];
                    ?>
                </td>
            </tr>
        </table>
        <br>
<?PHP
    }
}

?>