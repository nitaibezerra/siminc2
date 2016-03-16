<?php

include_once APPRAIZ . 'www/sase/_constantes.php';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'sase/classes/Assessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/AssessoramentoEstado.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

function posAcaoTramitacaoAssessoramento( $docid, $aseid = '' ){


	if (empty($aseid)){
		$Assessoramento = new Assessoramento();
		$Assessoramento->carregaAssessoramentoPeloDocid( $docid );
		$Assessoramento->stacod = array_search($Assessoramento->resgataEsdid(),$Assessoramento->situacaoEsdid);
        $Assessoramento->atualizarAssessoramento();
	} else {
		$ae = new AssessoramentoEstado();
		$ae->carregarPorId( $aseid );
		$ae->stacod = array_search($ae->resgataEsdid(),$ae->situacaoEsdid);
		$ae->atualizarAssessoramentoEstado();
	}

	return true;
}


function carregaMunicipioPelaUf($uf, $id){
	global $db;
	
	$sql = 'SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = \''.$uf.'\'';
	
	$db->monta_combo($id, $sql, 'S', 'Selecione...', '', '', '', '', 'N', '', '','','Município');
}

/**
 * Alinha o texto para a esquerda
 * @param mixed $valor Valor para ser formatado.
 * @return String
 */
function alinhaParaEsquerda($valor)
{
	$valor = "<p style=\"text-align: left !important;\">$valor</p>" ;
	return $valor;
}

/**
 * Alinha o texto para a direita
 * @param mixed $valor Valor para ser formatado.
 * @return String
 */
function alinhaParaDireita($valor)
{
	$valor = "<p style=\"text-align: right !important;\">$valor</p>" ;
	return $valor;
}

/**
 * Recupera o(s) perfil(is) do usuário no módulo
 *
 * @return array $pflcod
 */
function arrayPerfil($usucpf = null) {
    $usucpf = $usucpf != null ? $usucpf : $_SESSION['usucpf'];
	/*     * * Variável global de conexão com o bando de dados ** */
	global $db;

	/*     * * Executa a query para recuperar os perfis no módulo ** */
	$sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SASE_SISID . "
			WHERE
				pu.usucpf = '" . $usucpf . "'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna( $sql );

	/*     * * Retorna o array com o(s) perfil(is) ** */
	return (array) $pflcod;
}


//Verificar para a Ação:<Enviar para Análise pelo Coordenador Estadual> se o ES do registro possui Supervisor p liberar + função wfVerificaRelatorio
function wfVerificaRelatorioCoord($ravid, $usucpf, $ratdata1periodo, $ratdata2periodo){
     global $db;
    
    $verificaRelatorio = wfVerificaRelatorio($ravid, $usucpf, $ratdata1periodo, $ratdata2periodo);
   if($verificaRelatorio != 'true'){
       return $verificaRelatorio;
   }else{
       $sql = "SELECT trim(estuf)
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conEstado = $db->pegaUm($sql);
        $estSSupervisor = array(AC,AM,AP,RR,RO,DF,SE,MS,MT,ES,RJ,AL);

        if (!in_array($conEstado, $estSSupervisor)) {
            return "Este estado possui Supervisor.";
        }
        else{
             return true;
        }
   }
  
}

//Verificar para a Ação:<Enviar para Análise pelo Supervisor> se o ES do registro possui Supervisor p liberar + função wfVerificaRelatorio
function wfVerificaRelatorioSupervisor($ravid, $usucpf, $ratdata1periodo, $ratdata2periodo){
     global $db;
    
    $verificaRelatorio = wfVerificaRelatorio($ravid, $usucpf, $ratdata1periodo, $ratdata2periodo);
   if($verificaRelatorio != 'true'){
       return $verificaRelatorio;
   }else{
       $sql = "SELECT trim(estuf)
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conEstado = $db->pegaUm($sql);
        $estSSupervisor = array(AC,AM,AP,RR,RO,DF,SE,MS,MT,ES,RJ,AL);

        if(in_array($conEstado, $estSSupervisor)) {
            return "Este estado não possui Supervisor.";
        }else{
              return true;  
        }
   }
  
}

function wfVerificaRelatorio($ravid, $usucpf, $ratdata1periodo, $ratdata2periodo){
    global $db;

    // Verifica as atividades envolvidas
    $sql = "select count(ravid) count from sase.ativdesenvavaliadoresredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){
        return "Por favor, preencha o quadro 3, referente as Atividades Desenvolvidas.";
    }

    // Verifica os resultados consolidados
    $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SASE_SISID . "
			WHERE
				pu.usucpf = '" . $usucpf . "'
			ORDER BY
				p.pflnivel";
    $pfls = $db->carregarColuna( $sql );

    if (is_array($pfls)){
        if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
            $pflcod = PFLCOD_SASE_TECNICO;
        } else if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
            $pflcod = PFLCOD_SASE_SUPERVISOR;
        } else if (in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $pfls)) {
            $pflcod = PFLCOD_SASE_SUPERVISOR_GERAL;
        } else if (in_array(PFLCOD_SASE_EXECUTIVO, $pfls)){
            $pflcod = PFLCOD_SASE_EXECUTIVO;
        }
    }

    switch ($pflcod) {
        case PFLCOD_SASE_EXECUTIVO:
            $v = sfVerificaRelatorioCoordenador($ravid);
            break;

        case PFLCOD_SASE_SUPERVISOR_GERAL:
            $v = wfVerificaRelatorioSupervisorGeral($ravid);
            break;

        case PFLCOD_SASE_TECNICO:
        case PFLCOD_SASE_SUPERVISOR:
            $v = wfVerificaRelatorioTecnico($pflcod, $ravid, $ratdata1periodo, $ratdata2periodo, $usucpf);
        break;
    }

    if (!$v){
        return $v;
    } else {
        return true;
    }
}

function sfVerificaRelatorioCoordenador($ravid){
    global $db;

    // Valida as atividades executadas no período
    $sql = "select count(avaid) from sase.ativdesenvavaliadoresredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Atividades Executadas no Período."; }

    // Valida as informações a respeito do pleno estadual de educação
    $sql = "select count(ipeid) from sase.informacoespeesupgeralredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Informações a respeito do Plano Estadual de Educação."; }

    // Verifica as ações propostas pelos AE Supervisores para os municípios sem informação, sem comissão instituída e sem alteração de etapa de trabalho.
    $sql = "select count(apmid) from sase.acoespropmunseminfsupgeralredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Açoes propostas pelos AE Supervisores para os municípios sem informação, sem comissão instituída e sem alteração de etapa de trabalho."; }

    return true;
}

function wfVerificaRelatorioSupervisorGeral($ravid){
    global $db;

    // Valida as atividades executadas no período
    $sql = "select count(avaid) from sase.ativdesenvavaliadoresredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Atividades Executadas no Período."; }

    // Valida as informações a respeito do pleno estadual de educação
    $sql = "select count(ipeid) from sase.informacoespeesupgeralredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Informações a respeito do Plano Estadual de Educação."; }

    // Verifica as ações propostas pelos AE Supervisores para os municípios sem informação, sem comissão instituída e sem alteração de etapa de trabalho.
    $sql = "select count(apmid) from sase.acoespropmunseminfsupgeralredeae where ravid = {$ravid}";
    $con = $db->pegaUm($sql);
    if ($con == 0){ return "Por favor, informe as Açoes propostas pelos AE Supervisores para os municípios sem informação, sem comissão instituída e sem alteração de etapa de trabalho."; }

    return true;
}

function wfVerificaRelatorioTecnico($pflcod, $ravid, $ratdata1periodo, $ratdata2periodo, $usucpf){
    global $db;
    if($pflcod == PFLCOD_SASE_SUPERVISOR) {
        //pega os ravid dos tecnicos supervisionados

        $sql =	 "select tsp.cpftecnico
                                from  sase.tecnicosupervisionadosredeae tsp
                                where ravid = {$ravid} and  tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
                                group by  tsp.cpftecnico";

        $cpfTecnicos = $db->carregarColuna($sql);

        $ratdata1periodo2 = formata_data_sql($ratdata1periodo);
        $ratdata2periodo2 = formata_data_sql($ratdata2periodo);

        if ($cpfTecnicos) {
            $sql = "SELECT ravid FROM sase.relatorioavaliadorredeae
                                where usucpf in ('" . implode("','", $cpfTecnicos) . "') and ratdata1periodo = '{$ratdata1periodo2}' and ratdata2periodo = '{$ratdata2periodo2}'";
            $arrayRavid = $db->carregarColuna($sql);
        }

        if (is_array($arrayRavid) && count($arrayRavid) > 0) {
            $sql = "select
                            count(stmid) count
                        from sase.sitatualmunicipiotecredeae stm
                        inner join sase.municipiosassistidosredeae mar on mar.marid = stm.marid
                        where mar.ravid in (" . implode(",", $arrayRavid) . ")
                        --and stm.stmobservacoes is not null
                        --and trim(stm.stmobservacoes) != ''";
        } else {
            $sql = "select
                            count(stmid) count
                        from sase.sitatualmunicipiotecredeae stm
                        inner join sase.municipiosassistidosredeae mar on mar.marid = stm.marid
                        where mar.ravid = {$ravid}
                        --and stm.stmobservacoes is not null
                        --and trim(stm.stmobservacoes) != ''";
        }
    }else{
        $sql = "select
                            count(stmid) count
                        from sase.sitatualmunicipiotecredeae stm
                        inner join sase.municipiosassistidosredeae mar on mar.marid = stm.marid
                        where mar.ravid = {$ravid}
                        --and stm.stmobservacoes is not null
                        --and trim(stm.stmobservacoes) != ''";
    }
    $con = $db->pegaUm($sql);
    if ($con == 0){
        return "Por favor, preencha o quadro 4, referente aos Resultados Consolidados.";
    }

    // Verifica os Municípios assistidos com alteração
    // N/A

    // Verifica os municípios assistidos sem alteração de etapa
    $sql = "select
                            count(sta.stadsc)
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid
                        left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and ur.rpustatus = 'A'
                        and not exists (
                            select
                                1
                            from workflow.historicodocumento doc
                            where doc.docid = ass.docid
                            and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
                        )";
    $con = $db->pegaUm($sql);
    if ($con > 0) {
        if($pflcod == PFLCOD_SASE_SUPERVISOR) {
            if (is_array($arrayRavid) && count($arrayRavid) > 0) {
                $sql = "select
                                count(mstid) count
                            from sase.municipiossemalteracaoperiodotecredeae mst
                            inner join sase.municipiosassistidosredeae mar on mar.marid = mst.marid
                            where mar.ravid in (" . implode(",", $arrayRavid) . ")";
            } else {
                $sql = "select
                                count(mstid) count
                            from sase.municipiossemalteracaoperiodotecredeae mst
                            inner join sase.municipiosassistidosredeae mar on mar.marid = mst.marid
                            where mar.ravid = {$ravid}";
            }
        }else{
            $sql = "select
                                count(mstid) count
                            from sase.municipiossemalteracaoperiodotecredeae mst
                            inner join sase.municipiosassistidosredeae mar on mar.marid = mst.marid
                            where mar.ravid = {$ravid}";
        }
        $con = $db->pegaUm($sql);
        if ($con == 0) {
            return "Por favor, preencha o quadro 6, referente aos Municípios assistidos sem alteração de etapa.";
        }
    }

    // Verifica os municipios assistidos sem informação ou sem comissao instituída
    $sql = "select
                          count(ass.assid)
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and sta.esdid in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                        and ur.rpustatus = 'A'";
    $con = $db->pegaUm($sql);
    if ($con > 0){
        $sql = "select
                            count(msfid) count
                        from sase.municipiosseminftecredeae msf
                        inner join sase.municipiosassistidosredeae mar on mar.marid = msf.marid
                        where mar.ravid = {$ravid}";
        $con = $db->pegaUm($sql);
        if ($con == 0){
            return "Por favor, preencha o quadro 7, referente aos municípios assistidos sem informaçao ou sem comissao instituida.";
        }
    }

    // Verifica as ações propostas para os munícipios sem informações
    $sql = "select
                          count(ass.assid)
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and sta.esdid in (".ESDID_SASE_SEM_INFORMACAO.")
                        and ur.rpustatus = 'A'";
    $con = $db->pegaUm($sql);
    if ($con > 0){
        $sql = "select
                        count(msaid) count
                    from sase.municipiossemacoesdesenvstecredeaee msa
                    inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                    where mar.ravid = {$ravid}
                    and msaquadro = 1";
        $con = $db->pegaUm($sql);
        if ($con == 0){
            return "Por favor, preencha o quadro 8, referente às ações propostas para os Municípios sem informação.";
        }
    }

    // Verifica as ações propostas para os municípios sem comissão instituída
    $sql = "select
                          count(ass.assid)
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and sta.esdid in (".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                        and ur.rpustatus = 'A'";
    $con = $db->pegaUm($sql);
    if ($con > 0){
        $sql = "select
                        count(msaid) count
                    from sase.municipiossemacoesdesenvstecredeaee msa
                    inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                    where mar.ravid = {$ravid}
                    and msaquadro = 2";
        $con = $db->pegaUm($sql);
        if ($con == 0){
            return "Por favor, preencha o quadro 9, referente às ações propostas para os Municípios sem comissão instituída.";
        }
    }

    // Verifica as ações propostas para os municípios sem alteração de etapa de trabalho, mas com demora justificável
    $sql = "select distinct
                          count(ass.assid) count
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid
                        left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and ur.rpustatus = 'A'
                        and not exists (
                            select
                                1
                            from workflow.historicodocumento doc
                            where doc.docid = ass.docid
                            and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
                        )
                        and not exists (
                            select
                                1
                            from sase.municipiossemacoesdesenvstecredeaee msa
                            inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                            inner join sase.assessoramento ass on ass.assid = mar.assid
                            inner join territorios.municipio mun2 on mun.muncod = ass.muncod
                            where msaquadro = 4 and ravid = {$ravid} and mun2.muncod = mun.muncod
                        )";
    $con = $db->pegaUm($sql);
    if ($con > 0){
        $sql = "select
                        count(msaid) count
                    from sase.municipiossemacoesdesenvstecredeaee msa
                    inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                    where mar.ravid = {$ravid}
                    and msaquadro = 3";
        $con = $db->pegaUm($sql);
        if ($con == 0){
            return "Por favor, preencha o quadro 10, referente às ações propostas para os Municípios sem alteração de etapa de trabalho, mas com demora justificável.";
        }
    }

    // Verifica as açoes propostas para os Municípios sem alteração de etapa de trabalho com demora maior do que a esperada
    $sql = "select distinct
                            count(ass.assid) count
                        from sase.assessoramento ass
                        inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                        left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid
                        left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                        inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                        inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        where ur.usucpf = '{$usucpf}'
                        and ur.rpustatus = 'A'
                        and not exists (
                            select
                                1
                            from workflow.historicodocumento doc
                            where doc.docid = ass.docid
                            and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
                        )
                        and not exists (
                            select
                                1
                            from sase.municipiossemacoesdesenvstecredeaee msa
                            inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                            inner join sase.assessoramento ass on ass.assid = mar.assid
                            inner join territorios.municipio mun2 on mun.muncod = ass.muncod
                            where msaquadro = 3 and ravid = {$ravid} and mun2.muncod = mun.muncod
                        )";
    $con = $db->pegaUm($sql);
    if ($con > 0){
        $sql = "select
                        count(msaid) count
                    from sase.municipiossemacoesdesenvstecredeaee msa
                    inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                    where mar.ravid = {$ravid}
                    and msaquadro = 4";
        $con = $db->pegaUm($sql);
        if ($con == 0){
            return "Por favor, preencha o quadro 11, referente às ações propostas para os Municípios sem alteração de etapa de trabalho com demora maior do que a esperada.";
        }
    }
    return true;
}

 //Verifica o perfil que o tecnico associado tem para liberar tramitação somente para este perfil
    function wfVerificaPerfilTecnico($ravid) {
        global $db;
        $sql = "SELECT tae.taedsc
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conPerfil = $db->pegaUm($sql);
        if ($conPerfil == 'Técnico') {
            return true;
        }else{
            return "Avaliador Educacional é um ".$conPerfil.".";
        }
    }
    
    //Verifica o perfil que o tecnico associado tem para liberar tramitação somente para este perfil
    function wfVerificaPerfilSupervisor($ravid) {
        global $db;
        $sql = "SELECT tae.taedsc
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conPerfil = $db->pegaUm($sql);
        if ($conPerfil == 'Supervisor') {
            return true;
        }else{
            return "Avaliador Educacional é um ".$conPerfil.".";
        }
    }
    
       //Verifica o perfil que o tecnico associado tem para liberar tramitação somente para este perfil
    function wfVerificaPerfilSupervisorGeral($ravid) {
        global $db;
        $sql = "SELECT tae.taedsc
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conPerfil = $db->pegaUm($sql);
        if ($conPerfil == 'Supervisor Geral') {
            return true;
        }else{
            return "Avaliador Educacional é um ".$conPerfil.".";
        }
    }

       //Verifica o perfil que o tecnico associado tem para liberar tramitação somente para este perfil
    function wfVerificaPerfilCoordenadorEstadual($ravid) {
        global $db;
        $sql = "SELECT tae.taedsc
                  FROM sase.relatorioavaliadorredeae rav
                  inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                  inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                  inner join sase.tipoavaleducacional tae on ave.taeid  = tae.taeid
                where ravid = '{$ravid}'";

        $conPerfil = $db->pegaUm($sql);
        if ($conPerfil == 'Coordenador Estadual') {
            return true;
        }else{
            return "Avaliador Educacional é um ".$conPerfil.".";
        }
    }
    
function validaData($data){
    $dat = explode('/', $data);
    $d = $dat[0];
    $m = $dat[1];
    $y = $dat[2];
    //ver($dat, d);
    if (in_array('', $dat)){
        return false;
    } else {
        if (@checkdate($m, $d, $y)) {
            return true;
        } else {
            return false;
        }
    }
}

function validaUrlPerfil($origem, $usucpf = null, $diretoria = 1){
    $url = '';
    $pfls = arrayPerfil($usucpf);
    switch ($origem) {
        case 'RelatorioAvaliador':
            $url = 'sase.php?modulo=relatorio/relResultPlanEduc&acao=A&diretoria='.$diretoria;

            if (in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $pfls)){
                $url = 'sase.php?modulo=relatorio/relResultPESuperGeral&acao=A&diretoria='.$diretoria;
            }

            if (in_array(PFLCOD_SASE_TECNICO_DIVAPE, $pfls)){
                $url = 'sase.php?modulo=principal/planodecarreira/relatorios/relPDCTecnico&acao=A&diretoria='.$diretoria;
            }

            if (in_array(PFLCOD_SASE_EXECUTIVO, $pfls)){
                switch ($diretoria){
                    case 1:
                        $url = 'sase.php?modulo=relatorio/relResultPECoordenador&acao=A&diretoria='.$diretoria;
                        break;
                    case 2:
                        $url = 'sase.php?modulo=princpal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria='.$diretoria;
                        break;
                }
            }

//            if (in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls)){
//                switch ($diretoria){
//                    case 1:
//                        $url = 'sase.php?modulo=relatorio/relResultPECoordenador&acao=A&diretoria='.$diretoria;
//                        break;
//                    case 2:
//                        $url = 'sase.php?modulo=principal/planodecarreira/relatorios/relPECoordenador&acao=A&diretoria='.$diretoria;
//                        break;
//                }
//            }
        break;
    }
    return $url;
}

function cadastraDocumentoAE($pflcod, $docdsc, $esdid=null){
    global $db;
    // Determina o TPDID utilizado, que no caso será o de Avaliador Educacional.
    $tpdid = (integer) TPDID_SASE_AVALIADOREDUCACIONAL;
    $docdsc = str_replace( "'", "\\'", $docdsc );
    // Verifica o ESDID utilizado, pelo perfil informado nos parâmetros.
    if ($esdid == null) {
        switch ($pflcod) {
            case PFLCOD_SASE_TECNICO:
                $esdid = ESDID_SASE_EM_CADASTRAMENTO_PELO_TECNICO;
                break;
            case PFLCOD_SASE_SUPERVISOR:
                $esdid = ESDID_SASE_EM_CADASTRAMENTO_PELO_SUPERVISOR;
                break;
            case PFLCOD_SASE_SUPERVISOR_GERAL:
                $esdid = ESDID_SASE_EM_CADASTRAMENTO_PELO_SUPERVISOR_GERAL;
                break;
            case PFLCOD_SASE_EXECUTIVO:
                $esdid = ESDID_SASE_EM_CADASTRAMENTO_PELO_COORDENADOR_ESTADUAL;
                break;
            default:
                $esdid = ESDID_SASE_EM_CADASTRAMENTO_PELO_TECNICO;
                break;
        }
    }

    // grava documento
    $sql = "
		insert into workflow.documento
		( tpdid, esdid, docdsc )
		values ( " . $tpdid . ", " . $esdid . ", '" . $docdsc . "' )
		returning docid
	";

    $docid = $db->pegaUm( $sql );

    return $docid ? $docid : null;
}

function pegaPerfil($usucpf){
	global $db;

	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND
			pu.usucpf = '$usucpf'";


	$pflcod = $db->carregar( $sql );
	return $pflcod;
}

function getLegendaAcompanhamentoPne($subid){
    global $db;
    $html = <<<HTML
        <ul>
HTML;
    $sql = <<<DML
        select
            legcor as cor,
            legfxa1||'% - '||legfxa2||'%' as descricao
        from sase.legendaindicadores
        where subid = {$subid}
        order by legfxa1
DML;
    $lis = $db->carregar($sql);
    foreach ($lis as $key => $value) {
        $html .= <<<HTML
            <li>
                <table>
                    <tr>
                        <td>
                            <span style='background:{$value['cor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b></b>&nbsp;&nbsp;
                        </td>
                        <td>
                            {$value['descricao']}
                        </td>
                    </tr>
                </table>
            </li>
HTML;
    }
    $html .= <<<HTML
        </ul>
HTML;
    return $html;
}

function filtraArray($array, $campo, $string){
    return($array[$campo] == $string);
}

/**
 * Verifica os perfis de acesso do usuário
 *
 * @param $pflcods - Perfis que serão validados.
 * @return bool - Retorna TRUE ou FALSE caso o usuário possua algum dos perfis passados.
 */
function possui_perfil($pflcods){
    global $db;

    if (is_array($pflcods)) {
        $pflcods = array_map("intval", $pflcods);
        $pflcods = array_unique($pflcods);
    } else {
        $pflcods = array((integer) $pflcods);
    }
    if (count($pflcods) == 0) {
        return false;
    }

    $pflstr = implode(",", $pflcods);

    $sql = <<<DML
        select
            count(*)
        from seguranca.perfilusuario
        where usucpf = '{$_SESSION['usucpf']}'
        and pflcod in ( {$pflstr} );
DML;
    return $db->pegaUm($sql) > 0;
}

/**
 * Redireciona para a tela inicial, dependendo do perfil do usuário.
 */
function redirecionaTelaInicial(){
    global $db;

    $perfilDIVAPE = array(
        PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE,
        PFLCOD_SASE_TECNICO_DIVAPE
    );

    if(!possui_perfil(PFLCOD_SASE_SUPER_USUARIO) && possui_perfil($perfilDIVAPE)){
        header('Location: sase.php?modulo=principal/planodecarreira&acao=A');
    } else {
        header('Location: sase.php?modulo=principal/assessoramento&acao=A');
    }
    exit;
}

function retornaCampo($valor, $name, $disabled = false, $title = ""){
    $d = $disabled ? "disabled" : "";
    $t = $title != "" ? "title=\"{$title}\"" : "";
    return <<<HTML
        <input type="text" class="campoPorcentagem form-control" name="{$name}[]" id="{$name}" {$d} {$t} value="{$valor}"/>
HTML;
}

function retornaCampoHidden($valor, $name){
    return <<<HTML
        <input type="hidden" id="{$name}" name="{$name}[]" value="{$valor}"/>
HTML;
}

function retornaCampoPerDispersao($valor, $linha){
    $retorno = "";
    $retorno .= retornaCampoHidden($linha['marid'], 'maridq6');
    $retorno .= retornaCampoHidden($linha['esdid'], 'esdidq6');
    $retorno .= retornaCampoHidden($linha['ecrid'], 'ecridq6');
    $disabled = $linha['esdid'] == '' ? true : false;
    $mensagem = "Situação, do plano de carreira, não informada, no período do relatório";
    $retorno .= retornaCampo($valor, 'ecrpercentdispersao', $disabled, $mensagem);
    return $retorno;
}

function retornaCampoNecesHorasDocente($valor, $linha){
    $disabled = $linha['esdid'] == '' ? true : false;
    $mensagem = "Situação, do plano de carreira, não informada, no período do relatório";
    return retornaCampo($valor, 'ecrneceshorasdocente', $disabled, $mensagem);
}

function retornaCampoRelProfAluno($valor, $linha){
    $disabled = $linha['esdid'] == '' ? true : false;
    $mensagem = "Situação, do plano de carreira, não informada, no período do relatório";
    return retornaCampo($valor, 'ecrrelprofaluno', $disabled, $mensagem);
}

function retornaCampoHorasDocenteContrat($valor, $linha){
    $disabled = $linha['esdid'] == '' ? true : false;
    $mensagem = "Situação, do plano de carreira, não informada, no período do relatório";
    return retornaCampo($valor, 'ecrhorasdocentecontrat', $disabled, $mensagem);
}

function retornaColunaMunicipioQuadro4PDC($valor, $linha){
    $retorno = "";
    $retorno .= retornaCampoHidden($linha['marid'], 'maridq4');
    $retorno .= retornaCampoHidden($linha['matid'], 'matidq4');
    $retorno .= retornaCampoHidden($linha['esdorigem'], 'esdorigemq4');
    $retorno .= retornaCampoHidden($linha['esddestino'], 'esddestinoq4');
    $retorno .= $valor;
    return $retorno;
}