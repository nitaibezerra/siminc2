<?PHP
    function atualizaComboMunicipio($dados){
        global $db;

        extract($dados);

        $sql = "
            SELECT  muncod AS codigo,
                    mundescricao AS descricao
            FROM territorios.municipio
            WHERE estuf = '{$estuf}'
            ORDER BY mundescricao
        ";

        if($tipo == 'S'){
            $db->monta_combo('muncodend_s', $sql, 'S', 'Selecione...','','','','372','S','muncodend_s','', $muncod);
        } else {
            $db->monta_combo('muncodend_m',$sql,'S','Selecione...','','','','372','S','muncodend_m','', $muncod);
        }
        die();
    }

    function buscaDadosPrefeito(){
        global $db;

        $sql = "
            SELECT  e.entid,
                    e.entnome,
                    e.entnumcpfcnpj,
                    iu.muncod,
                    m.estuf,
                    m.mundescricao,
                    e.endlog,
                    e.endbai,
                    e.entnumrg,
                    e.entorgaoexpedidor,
                    est.estdescricao,
                    e.entstatus,
                    'Prefeito(a)' as cargo_func,
                    'Prefeitura' as orgao,
                    e.entdatanasc,
                    e.entnumdddcomercial,
                    e.entnumcomercial,
                    e.entnumdddcelular,
                    e.entnumcelular,
                    e.entemail
            FROM par.entidade e

            JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
            JOIN territorios.municipio m ON m.muncod = iu.muncod
            JOIN territorios.estado est ON est.estuf = m.estuf

            WHERE iu.muncod = '{$_SESSION['par']['muncod']}' AND e.dutid = ".DUTID_PREFEITO;

            return $prefeito = $db->pegaLinha($sql);
    }

    function buscaDadosGestorMaisMedicos_novo_2015($tipo){
	global $db;

        $sql = "
            SELECT  dmmid,
                    prgid,
                    m.muncod,
                    muncodend,
                    c.estuf,
                    dmmnome,
                    trim( replace(to_char(cast(dmmcpf as bigint), '000:000:000-00'), ':', '.') ) AS dmmcpf,
                    dmmrg,
                    dmmsexo,
                    dmmdtnascimento,
                    dmmorgao,
                    dmmfonecomercial,
                    dmmcelular,
                    dmmemail,
                    dmmcargofuncao
            FROM par.dadosmaismedicos m
            LEFT JOIN territorios.municipio AS c ON c.muncod = m.muncodend
            WHERE m.muncod = '{$_SESSION['par']['muncod']}' AND dmmtipo = '$tipo' AND prgid = ".PROG_PAR_MAIS_MEDICO_NOVO_2015."
        ";
        $dados = $db->pegaLinha($sql);
        return $dados;
    }

    function continuaAdesao( $dados ){
	global $db;
	extract($dados);

	$sql = "
            INSERT INTO par.respquestaomaismedico(
                    muncod, prgid, rqmquestao03, rqmquestao04, rqmquestao05, rqmquestao06, usucpf
                )VALUES(
                    '{$muncod}', {$prgid}, '{$rqmquestao03}', '{$rqmquestao04}', '{$rqmquestao05}', {$rqmquestao06}, '{$_SESSION['usucpf']}'
            ) RETURNING rqmid
        ";
	$rqmid = $db->pegaUm($sql);

	if($rqmid){
            $_SESSION['par']['rqmid'] = $rqmid;
            $_SESSION['continuaAdesaoMaisMedico_2015'] = 'S';
            $db->commit();
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/informacao_municipio','', 'Operação realizado com sucesso. Passe a proxima etapa' );
            return $rqmid;
	} else {
            $_SESSION['continuaAdesaoMaisMedico_2015'] = 'N';
            $db->rollback();
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao','', 'Não foi possível executar a operação, tente novamente mais tarde!' );
	}
    }

    function excluirDoc($dados){
        global $db;

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $arqid = $dados;

        if ($arqid != '') {
            $sql = "UPDATE par.arquivosmunicipio SET aqmsituacao = 'I' WHERE arqid = {$arqid} ";
        }

        if( $db->executar($sql) ){
            $file = new FilesSimec('arquivosmunicipio', $campos, 'par');
            $file->excluiArquivoFisico( $arqid );

            $db->commit();
        }
    }

    function exibirListaDoc(){
        global $db;

        $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

        $habilitado = verificaDataFechamento();

        $where = "am.aqmsituacao = 'A'";

        if ($_SESSION['par']['rqmid']) {
            $where .= " AND am.rqmid = {$_SESSION['par']['rqmid']}";
        }

        if ($_SESSION['par']['muncod']) {
            $where .= " AND rm.muncod = '{$_SESSION['par']['muncod']}'";
        }

        if( ( in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) ||   in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ) ){
            $acao = "
                <a href=\"par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A&download=S&arqid='|| am.arqid ||'\" >
                    <img src=\"../imagens/anexo.gif\" border=\"0\">
                </a>
                <img border=\"0\" src=\"../imagens/excluir_01.gif\" id=\"'|| am.arqid ||'\"/>
            ";
        } else {
            if( $habilitado == 'S' ){
                $acao = "
                    <a href=\"par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A&download=S&arqid='|| am.arqid ||'\" >
                        <img src=\"../imagens/anexo.gif\" border=\"0\">
                    </a>
                    <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| am.arqid ||'\" onclick=\"excluirDoc('|| am.arqid ||');\" style=\"cursor:pointer;\"/>
                ";
            }else{
                $acao = "
                    <a href=\"par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/documentos_ad&acao=A&download=S&arqid='|| am.arqid ||'\" >
                        <img src=\"../imagens/anexo.gif\" border=\"0\">
                    </a>
                    <img border=\"0\" src=\"../imagens/excluir_01.gif\" id=\"'|| am.arqid ||'\"/>
                ";
            }
        }

        $sql = "
            SELECT  '{$acao}' AS acao,
                    ar.arqnome||'.'||ar.arqextensao AS nome_arquivo,
                    ar.arqdescricao,
                    ta.tpadsc,
                    us.usunome
            FROM par.arquivosmunicipio am

            JOIN par.respquestaomaismedico rm ON am.rqmid = rm.rqmid
            JOIN public.arquivo ar ON ar.arqid = am.arqid
            JOIN par.tipoarquivo ta ON ta.tpaid = am.tpaid
            JOIN seguranca.usuario us ON us.usucpf = ar.usucpf

            WHERE {$where}

            ORDER BY am.aqmdtinclusao DESC
        ";
        $alinhamento = array('center','left','left','left','left');
        $tamanho = array('5%','18%','18%','18%','18%');
        $cabecalho = array('Ação', 'Nome Arquivo','Descrição','Tipo de Arquivo','Responsável');
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '',$tamanho, $alinhamento);
    }

    function informacaoMunicipios_2015(){
        global $db;

        $ifmcodibge = substr( $_SESSION['par']['muncod'], 0, 6 );

        $sql = "
            SELECT  ifmid,
                    ifmcodibge,
                    ifmpopulacao,
                    ifmtotalsus,
                    ifmeab,
                    ifmhospleito,
                    ifmpmaq,
                    ifmtotalcaps
            FROM maismedicomec.infomunicipio
            WHERE ifmcodibge = '{$ifmcodibge}'
        ";
        $rs = $db->pegaLinha($sql);
        return $rs;
    }

    function listarParceiro(){
        global $db;

        $acao = "
            <img src=\"../imagens/alterar.gif\" id=\"' || pm.pmmid ||'\" class=\"alterar\" onclick=\"alterarParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
            <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| pm.pmmid ||'\" onclick=\"excluirParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
        ";

        $acao_anexo = "
            <img src=\"../imagens/alterar.gif\" id=\"' || pm.pmmid ||'\" class=\"alterar\" onclick=\"alterarParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
            <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| pm.pmmid ||'\" onclick=\"excluirParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
            <a href=\"par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A&download=S&arqid='|| ar.arqid ||'\"/><img src=\"../imagens/anexo.gif\" border=\"0\"></a>
        ";

        $sql = "SELECT  CASE WHEN pm.arqid IS NULL
                        THEN '$acao'
                        ELSE '$acao_anexo'
                    END AS acao,

                    '<img border=\"0\" style=\"vertical-align:middle;cursor:pointer;\" src=\"../imagens/consultar.gif\" onclick=\"abrirTermo('|| pm.pmmid ||');\">' AS termo,

                    tm.mundescricao,
                    pmmnumleitos,

                    CASE WHEN pm.arqid IS NOT NULL
                        THEN ar.arqnome||'.'||ar.arqextensao
                        ELSE '<input type=\"file\" name=\"arquivo_'||pm.muncod||'\" id=\"arquivo_'||pm.muncod||'\" />'
                    END AS nome_arquivo,

                    CASE WHEN pm.arqid IS NULL
                        THEN '<input type=\"button\" name=\"anexar_termo_'|| pm.muncod ||'\" id=\"anexar_termo_'|| pm.muncod ||'\" value=\"Anexar\" onclick=\"anexarTermoPerceria(\''|| pm.muncod ||'\');\" />'
                        ELSE '<input type=\"button\" name=\"anexar_termo\" id=\"anexar_termo\" value=\"Anexar\" disabled=\"disabled\"/>'
                    END AS botao

            FROM par.parcamaismedico pm

            LEFT JOIN public.arquivo ar ON ar.arqid = pm.arqid
            LEFT JOIN territorios.municipio tm ON substr(tm.muncod,1,6) = pm.muncod

            WHERE pm.muncod = '{$_SESSION['par']['muncod']}'

            ORDER BY tm.mundescricao
        ";
        $alinhamento = array('center','center','','center','center', 'center');
        $tamanho = Array('5%', '10%', '40%', '10%', '20%', '10%');
        $cabecalho = array('Ação','Termo de Parceria','Município','Nº Leitos da Parceria', 'Anexo', '');
        //$db->monta_lista($sql, $cabecalho, '50','10', '', '', '', 'formulario_regiao', $tamanho, $alinhamento);
        $db->monta_lista($sql, $cabecalho, '50','10', '', '', '', '', $tamanho, $alinhamento);
    }

    #VERIFICA SE O MUNICIPIPO OFERECE CURSO DE MEDICINA, SE CASO SIM, "SE O MUNICIPÍO FAZER PARTE DOS QUE ESTAO NA BASE", ELE NÃO TEM CONDIÇÕES DE PARTICIPAÇÕES.
    function possuiOfertaCurso(){
        global $db;

        $sql = "SELECT cmemuncod FROM par.cursosmedicinaemec WHERE cmemuncod = '{$_SESSION['par']['muncod']}'";

        $muncod = $db->pegaUm($sql);

        if($muncod > 0){
            $oferta = 't';
        } else {
            $oferta = 'f';
        }
        return $oferta;
    }

    function recuperarMunicipio() {
        global $db;

        $sql = "
            SELECT  mundescricao || ' - ' || estuf AS municipio
            FROM territorios.municipio
            WHERE muncod = '{$_SESSION['par']['muncod']}'
        ";
        $rs = $db->pegaUm($sql);
        return $rs;
    }

//    function salvarTermoResidencia($post){
//	global $db;
//
//	extract($post);
//
//	$sql = "
//            UPDATE par.respquestaomaismedico
//   		SET rqmaceitetermoresidencia = '{$rqmaceitetermoresidencia}'
//            WHERE rqmid = {$rqmid}
//        ";
//	$db->executar($sql);
//
//	if($db->commit()){
//            echo 'S';
//	} else {
//            echo 'N';
//	}
//    }

    function salvarInformacaoMum($post){
        global $db;

        extract($post);

        $rqmquestao01 = $rqmquestao01 ? "'{$rqmquestao01}'" : 'null';
        $rqmquestao02 = $rqmquestao02 ? "'{$rqmquestao02}'" : 'null';
        $rqmquestao07 = $rqmquestao07 ? $rqmquestao07 : 'null';

        $rqmquestao08 = $rqmquestao08 ? $rqmquestao08 : "f";
        $rqmquestao09 = $rqmquestao09 ? str_replace('.','',$rqmquestao09) : 'null';
        $rqmquestao10 = $rqmquestao10 ? $rqmquestao10 : "f";

        $rqmquestao10item1 = $rqmquestao10item1 ? $rqmquestao10item1 : "f";
        $rqmquestao10item2 = $rqmquestao10item2 ? $rqmquestao10item2 : "f";
        $rqmquestao10item3 = $rqmquestao10item3 ? $rqmquestao10item3 : "f";
        $rqmquestao10item4 = $rqmquestao10item4 ? $rqmquestao10item4 : "f";
        $rqmquestao10item5 = $rqmquestao10item5 ? $rqmquestao10item5 : "f";

        $rqmquestao11 = $rqmquestao11 ? $rqmquestao11 : "f";
        $rqmquestao12 = $rqmquestao12 ? $rqmquestao12 : "f";
        $rqmquestao13 = $rqmquestao13 ? $rqmquestao13 : "f";
        $rqmquestao14 = $rqmquestao14 ? $rqmquestao14 : "f";
        $rqmparecermec = $rqmparecermec ? "'{$rqmparecermec}'" : 'null';

        $rqmaceitetermoresidencia = 't';

        $sql = "
            UPDATE par.respquestaomaismedico
                SET rqmquestao01        = {$rqmquestao01},
                    rqmquestao02        = {$rqmquestao02},
                    rqmquestao07        = {$rqmquestao07},
                    rqmquestao08        = '{$rqmquestao08}',
                    rqmquestao09        = {$rqmquestao09},
                    rqmquestao10        = '{$rqmquestao10}',
                    rqmquestao10item1   = '{$rqmquestao10item1}',
                    rqmquestao10item2   = '{$rqmquestao10item2}',
                    rqmquestao10item3   = '{$rqmquestao10item3}',
                    rqmquestao10item4   = '{$rqmquestao10item4}',
                    rqmquestao10item5   = '{$rqmquestao10item5}',
                    rqmquestao11        = '{$rqmquestao11}',
                    rqmquestao12        = '{$rqmquestao12}',
                    rqmquestao13        = '{$rqmquestao13}',
                    rqmquestao14        = '{$rqmquestao14}',
                    rqmparecermec       = $rqmparecermec,
                    rqmparecermectexto  = '{$rqmparecermectexto}',
                    rqmaceitetermoresidencia = '{$rqmaceitetermoresidencia}'
                WHERE rqmid = {$rqmid}
        ";
        $db->executar($sql);

        if( $db->commit() ){
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/informacao_municipio', '', 'Termo de Compromisso Residência Médica aceito com sucesso!');
        }
    }

    function salvarDadosMaisMedicos( $dados ){
	global $db;

        extract($dados);

        $dmmdtnascimento_s  = formata_data_sql($dmmdtnascimento_s);
        $dmmcpf_s           = corrige_cpf($dmmcpf_s);
        $dmmdtnascimento_m  = formata_data_sql($dmmdtnascimento_m);
        $dmmcpf_m           = corrige_cpf($dmmcpf_m);
        $muncod             = $_SESSION['par']['muncod'];

        if($dmmid_s == '' || $dmmid_m == ''){
            $sql = "
                INSERT INTO par.dadosmaismedicos(
                        prgid, muncod, muncodend, dmmnome, dmmcpf, dmmrg, dmmsexo, dmmdtnascimento, dmmorgao, dmmfonecomercial, dmmcelular, dmmemail, dmmcargofuncao, dmmtipo, dmmstatus, dmmdtinclusao
                    )VALUES(
                        ".PROG_PAR_MAIS_MEDICO_NOVO_2015.", '{$muncod}', '{$muncodend_s}', '{$dmmnome_s}', '{$dmmcpf_s}', '{$dmmrg_s}', '{$dmmsexo_s}', '{$dmmdtnascimento_s}', '{$dmmorgao_s}', '{$dmmfonecomercial_s}', '{$dmmcelular_s}', '{$dmmemail_s}', '{$dmmcargofuncao_s}', 'S', 'A', 'NOW()'
                );
            ";

            $sql .= "
                INSERT INTO par.dadosmaismedicos(
                        prgid, muncod, muncodend, dmmnome, dmmcpf, dmmrg, dmmsexo, dmmdtnascimento, dmmorgao, dmmfonecomercial, dmmcelular, dmmemail, dmmcargofuncao, dmmtipo, dmmstatus, dmmdtinclusao
                    )VALUES(
                        ".PROG_PAR_MAIS_MEDICO_NOVO_2015.", '{$muncod}', '{$muncodend_m}', '{$dmmnome_m}', '{$dmmcpf_m}', '{$dmmrg_m}', '{$dmmsexo_m}', '{$dmmdtnascimento_m}', '{$dmmorgao_m}', '{$dmmfonecomercial_m}', '{$dmmcelular_m}', '{$dmmemail_m}', '{$dmmcargofuncao_m}', 'M', 'A', 'NOW()'
                )RETURNING dmmid;";

                $dmmid = $db->pegaUm($sql);
        } else {
            $sql = "
                UPDATE par.dadosmaismedicos SET
                    muncodend		= '{$muncodend_s}',
                    dmmnome		= '{$dmmnome_s}',
                    dmmcpf		= '{$dmmcpf_s}',
                    dmmrg		= '{$dmmrg_s}',
                    dmmsexo		= '{$dmmsexo_s}',
                    dmmdtnascimento	= '{$dmmdtnascimento_s}',
                    dmmorgao		= '{$dmmorgao_s}',
                    dmmfonecomercial    = '{$dmmfonecomercial_s}',
                    dmmcelular		= '{$dmmcelular_s}',
                    dmmemail		= '{$dmmemail_s}',
                    dmmcargofuncao	= '{$dmmcargofuncao_s}'
                WHERE dmmid = {$dmmid_s} AND muncod = '{$muncod}' AND prgid = ".PROG_PAR_MAIS_MEDICO_NOVO_2015." RETURNING dmmid;
            ";

            $sql .= "
                UPDATE par.dadosmaismedicos SET
                    muncodend		= '{$muncodend_m}',
                    dmmnome		= '{$dmmnome_m}',
                    dmmcpf		= '{$dmmcpf_m}',
                    dmmrg		= '{$dmmrg_m}',
                    dmmsexo		= '{$dmmsexo_m}',
                    dmmdtnascimento	= '{$dmmdtnascimento_m}',
                    dmmorgao		= '{$dmmorgao_m}',
                    dmmfonecomercial    = '{$dmmfonecomercial_m}',
                    dmmcelular		= '{$dmmcelular_m}',
                    dmmemail		= '{$dmmemail_m}',
                    dmmcargofuncao	= '{$dmmcargofuncao_m}'
                WHERE dmmid = {$dmmid_m} AND muncod = '{$muncod}' AND prgid = ".PROG_PAR_MAIS_MEDICO_NOVO_2015." RETURNING dmmid;
            ";
            $dmmid = $db->pegaUm($sql);
        }

        if($dmmid > 0 ){
            $db->commit();
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/dados_representantes' );
        }
    }

    function salvarAdesao($post = null){
	global $db;

	extract($post);

        $adpano = date('Y');

	$sql = "
            SELECT  adpid
            FROM par.pfadesaoprograma
            WHERE pfaid = {$_SESSION['par']['pfaid']} AND inuid = {$_SESSION['par']['inuid']}
        ";
	$adpid = $db->pegaUm($sql);

	if(!$adpid){
            $tapid = $tapid ? $tapid : 'null';

            $sql = "
                INSERT INTO par.pfadesaoprograma(
                        pfaid, adpano, inuid, adpdataresposta, adpresposta, usucpf, tapid
                    ) VALUES (
			{$_SESSION['par']['pfaid']}, '{$adpano}', {$_SESSION['par']['inuid']}, now(), '{$adpresposta}', '{$_SESSION['usucpf']}', {$tapid}
                ) RETURNING adpid
            ";
            $adpid = $db->pegaUm($sql);
        } else {
            $sql = "
                UPDATE par.pfadesaoprograma
                    SET adpdataresposta = now(),
                        adpresposta     = '{$adpresposta}',
                        usucpf          = '{$_SESSION['usucpf']}'
                WHERE adpid = {$adpid} RETURNING adpid
            ";
            $adpid = $db->pegaUm($sql);
        }

        if( $adpid > 0 ){
            $sql = "
                INSERT INTO par.pfadesaoprogramahistorico(
                        adpid, aphano, inuid, aphdataresposta, aphresposta, usucpf, tapid, pfaid
                    ) VALUES (
                        {$adpid}, '{$adpano}', {$_SESSION['par']['inuid']}, now(), '{$resposta}', '{$_SESSION['usucpf']}', {$tapid}, {$_SESSION['par']['pfaid']}
                ) RETURNING aphid
            ";
//ver(2, $sql, d);
            $aphid = $db->pegaUm($sql);
            if( $aphid > 0 ){
                if( $adpresposta == 'S' ){
                    $_SESSION['par']['adpid'] = $adpid;
                    $db->commit();
                    $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/adesao_programa', '', 'Termo aceito. Operação Realizada com sucesso!');
                }else{
                    $_SESSION['par']['adpid'] = $adpid;
                    $db->commit();
                    $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao', '', 'Operação Realizada com sucesso. Termo "NÃO ACEITO"!');
                }
            } else {
                $db->rollback();
                $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/adesao_programa', '', 'Ocorreu algum problema com a operação. Tente novamente mais tarde!');
            }
        } else {
            $db->rollback();
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/adesao_programa', '', 'Ocorreu algum problema com a operação. Tente novamente mais tarde!');
        }
    }

    function salvarDoc($file, $post){
        global $db;

        extract($post);

        if($file['arquivo']['tmp_name']){
            $aryCampos = array(
                "rqmid"         => $rqmid,
                "aqmsituacao"   => "'A'",
                "aqmdtinclusao" => "now()",
                "tpaid"         => $tpaid
            );
            $file = new FilesSimec("arquivosmunicipio", $aryCampos, "par");
            $file->setUpload(substr($arqdescricao, 0, 255), "arquivo");

            $resId = $file->getIdArquivo();

            if( $resId > 0 ){
                $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/documentos_ad', '', 'O Upload do arquivo foi realizado com sucesso!');
            }else{
                $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/documentos_ad', '', 'Não foi possível fazer o Upload do arquivo, tente novamente mais tarde!');
            }
            exit();
        } else {
            $db->sucesso( 'principal/programas/feirao_programas/mais_medicos_2015/documentos_ad', '', 'Ocorre algum problema com o arquivo. Tente novamente!');
            exit();
        }
    }

    function totalLeitos($pmmid = null){
        global $db;

        if($pmmid){
            $aryWhere[] = "pmmid = {$pmmid}";
        } else {
            if($_SESSION['par']['rqmid']){
                $aryWhere[] = "rqmid = {$_SESSION['par']['rqmid']}";
            }
        }

        $sql = "
            SELECT  SUM(pmmnumleitos) AS total_leitos
            FROM par.parcamaismedico
            ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
        ";
        $rs = $db->pegaUm($sql);
        return $rs;
    }

    function recuperaDadosPrefeitura(){
	global $db;

        $sql = "
            SELECT  ent1.entnome AS prefeito,
                    ent2.entnumcpfcnpj AS cnpjmunicipio,
                    mun.mundescricao AS municipio,
                    est.estdescricao AS estado,
                    mun.estuf AS estuf,
                    ent2.endlog || ' ' || ent2.endnum || ' ' || ent2.endbai || ' ' || 'CEP:' || ent2.endcep || ' ' || mun.mundescricao || '-' || mun.estuf AS endereco
            FROM par.entidade ent1

            INNER JOIN par.entidade ent2 ON ent1.inuid = ent2.inuid AND ent2.entstatus = 'A' AND ent2.dutid = ".DUTID_PREFEITURA."
            INNER JOIN territorios.municipio mun ON mun.muncod = ent2.muncod
            INNER JOIN territorios.estado est ON est.estuf = mun.estuf

            WHERE ent1.entstatus = 'A' AND ent1.dutid = ".DUTID_PREFEITO." AND ent2.muncod='{$_SESSION['par']['muncod']}'
        ";
        $rs = $db->pegaLinha($sql);
        return $rs;
    }

    function recuperaGestorSusMunicipio(){
	global $db;

	$sql = "
            SELECT dmmnome FROM par.dadosmaismedicos WHERE muncod = '{$_SESSION['par']['muncod']}' AND dmmtipo = 'S' AND prgid = ".PROG_PAR_MAIS_MEDICO_NOVO_2015."
        ";
	$rs = $db->pegaUm($sql);
	return $rs;
    }

    function validaPreenchimentoDados(){
        global $db;

        $sql = "SELECT MAX(dmmid) AS dmmid FROM par.dadosmaismedicos WHERE muncod = '{$_SESSION['par']['muncod']}' AND prgid = ".PROG_PAR_MAIS_MEDICO_NOVO_2015;
        $dmmid = $db->pegaUm($sql);

        $sql = "SELECT MAX(aqmid) AS aqmid FROM par.arquivosmunicipio WHERE rqmid = '{$_SESSION['par']['rqmid']}' ";
        $aqmid = $db->pegaUm($sql);

        if( $dmmid > 0 && $aqmid > 0 ){
            return true;
        }else{
            return false;
        }
    }

    function verificarRQMID(){
	global $db;

	$sql = "
            SELECT rqmid
            FROM par.respquestaomaismedico
            WHERE muncod = '{$_SESSION['par']['muncod']}' AND prgid = {$_SESSION['par']['prgid']}
        ";
	$rs = $db->pegaUm($sql);
	return $rs;
    }

    function verificarAdesao(){
	global $db;

	if( $_SESSION['par']['adpid'] ){
            $sql = "
                SELECT adpresposta
                FROM par.pfadesaoprograma
                WHERE adpid = {$_SESSION['par']['adpid']}
                GROUP BY adpid, adpresposta
                ORDER BY adpid DESC
            ";
            $adpresposta = $db->pegaUm($sql);
            return $adpresposta;
	} else {
            return 'N';
	}
    }

    function verificarVagas(){
        global $db;

        $sql = "
            SELECT  vgmnumvagashab, vgmmedicohab
            FROM par.vagasmedicos
            WHERE estuf = '{$_SESSION['par']['estuf']}'
        ";
        $rs = $db->pegaLinha($sql);
        return $rs;
    }

    function verificarDadosMunicipios(){
        global $db;

        $codigo_ibge = substr($_SESSION['par']['muncod'],0,6);

        $sql = "
            SELECT  leitos_sus::numeric AS leitos_sus,
                    vagas_pleiteadas,
                    leitos_abertura,
                    REPLACE(grau_comprometimento, ',', '.')::numeric AS grau_comprometimento,
                    leito_sus_vaga_municipio,
                    hospital_ensino,
                    residencia_medica::numeric AS residencia_medica,
                    hospital_100_leitos,
                    pronto_socorro,
                    adesao_pmaq,
                    caps,
                    REPLACE(equipe_atencao_basica,',','.')::numeric AS equipe_atencao_basica,
                    vagas_equipe
            FROM par.dadosmunicipios
            WHERE codigo_ibge = '{$codigo_ibge}'
        ";
        $rs = $db->pegaLinha($sql);
        return $rs;
    }

    function verificarPopulacao(){
	global $db;

	$muncod = substr($_SESSION['par']['muncod'],0,6);

	$sql = "SELECT popnumpopulacao FROM par.populacao WHERE popcodigomunicipio = '{$muncod}'";
	$rs = $db->pegaUm($sql);
	return $rs;
    }

    function verificarCapitalEstado(){
	global $db;

	$sql = "SELECT estuf FROM territorios.estado WHERE muncodcapital = '{$_SESSION['par']['muncod']}'";
	$rs = $db->pegaUm($sql);
        if( trim($rs) == '' ){
            $rs = 'f';
        }else{
            $rs = 't';
        }
	return $rs;
    }

    function verificaDataFechamento( $tipo = NULL ){
        global $db;
        
        $muncod = substr($_SESSION['par']['muncod'],0,6);

        #DATA DE FECHAMENTO PARA ADSÃO AO MAIS MÉDICO EDITAL 2015
        define("DATA_FECHAMENTO_ADESAO_MAIS_MEDICO_2015", '2015-06-17');
        
        #DATA DE FECHAMENTO PARA ADSÃO AO MAIS MÉDICO EDITAL 2015 PELO MUNICÍPIO DE RIBEIRA DO POMBAL
        define("DATA_FECHAMENTO_ADESAO_MAIS_MEDICO_RIBEIRO_POMBAL", '2015-06-17');
        
        #CONSTANTES DAS EXCECÕES - MUNICÍPIOS LIBERADOS PARA ADESÃO AO PROGRAMA FORA DO DA DATA PREVISTA.
        define("RIBEIRA_DO_POMBAL", 292660);

        #EXCECÕES - MUNICÍPIOS LIBERADOS PARA ADESÃO AO PROGRAMA FORA DO DA DATA PREVISTA.
        $array_muncod_exc = array(RIBEIRA_DO_POMBAL);

        if( !$db->testa_superuser() ){
        
            if( !in_array( $muncod, $array_muncod_exc ) ){
                if( ( strtotime( date('Y-m-d') ) <= strtotime( DATA_FECHAMENTO_ADESAO_MAIS_MEDICO_2015) ) ){
                    return 'S';
                }else{
                    return 'N';
                }
            }else{
                if( $muncod == RIBEIRA_DO_POMBAL ){
                    if( ( strtotime( date('Y-m-d') ) <= strtotime( DATA_FECHAMENTO_ADESAO_MAIS_MEDICO_RIBEIRO_POMBAL) ) ){
                        return 'S';
                    }else{
                        return 'N';
                    }
                }
            }
            
        }else{
            return 'S';
        }
    }

