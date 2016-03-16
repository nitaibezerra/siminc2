<?php
$permissao_formulario           = 'N';
$permissao_formulario_detalhe   = 'S';

if($_REQUEST['act'] == 'carregaPartido'){

        $tauid = $_REQUEST['tauid'];
		$modelo = new PartidoPolitico();
		$dados  = $modelo->getPartidosByTipoAutor($tauid);
		$db->monta_combo("ppoid",$dados,$permissao_formulario_detalhe,"&nbsp;",'carregaAutor();','','','440','S',"ppoid",false,$_REQUEST['ppoid']);
		die();
}

if($_REQUEST['act'] == 'carregaAutor'){

		$ppoid = $_REQUEST['ppoid'];

		$modelo = new AutorEmenda();
		$dados  = $modelo->getAutorEmendaByPartidoPolitico($ppoid);
		$db->monta_combo("aueid",$dados,$permissao_formulario_detalhe,"&nbsp;",'','','','440','S',"aueid",false,$_REQUEST['aueid']);
		die();
}

if( $_REQUEST['act'] == 'filtraPlanoOrcamentario'){

	//recuperando o $acaid
	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}

	$modelo = new PlanoOrcamentario();
	$dados  = $modelo->recuperarTodos("ploid AS codigo, plocod || ' - ' || plonome AS descricao", array("plostatus = 'A'","acaid = {$acaid}"), "descricao");
    $db->monta_combo("ploid", $dados, 'S', '&nbsp', '', '', '', '570', 'S', 'ploid');

	die();
}


if( $_REQUEST['act'] == 'validaPrioridade'){

	$retorno = 0;

	if( !empty( $_REQUEST['prioridade'] ) ){
		$projetoPlanejamento = new ProjetoPlanejamento();
		$retorno = count($projetoPlanejamento->validaPrioridade( $_REQUEST['prioridade'] , $_REQUEST['unidade'], $_REQUEST['pplid']));
	}

	echo $retorno;
	die();
}

if( $_REQUEST['act'] == 'atualizaPrioridades'){


	if( !empty( $_REQUEST['prioridade'] ) ){
		$projetoPlanejamento = new ProjetoPlanejamento();
		$retorno = $projetoPlanejamento->validaPrioridade( $_REQUEST['prioridade'] , $_REQUEST['unidade'], $_REQUEST['pplid']);

		if( is_array($retorno) && !empty($retorno)){
			foreach ($retorno as $dado){
				$dados 		  			= array();
				$dados['pplid'] 		= (int) $dado;
				$dados['dplprioridade'] = null;
				$nulos[]				= dplprioridade;

				$projetoPlanejamento->popularDadosObjeto($dados)->salvar(null,null, $nulos);
				$projetoPlanejamento->setDadosNull();
			}
			$projetoPlanejamento->commit();
		}
	}
	echo 'true';
	die();
}?>

<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
</script>

<?php

$arMes = array('1'  => 'Janeiro',
               '2'  => 'Fevereiro',
               '3'  => 'Março',
               '4'  => 'Abril',
               '5'  => 'Maio',
               '6'  => 'Junho',
               '7'  => 'Julho',
               '8'  => 'Agosto',
               '9'  => 'Setembro',
               '10' => 'Outubro',
               '11' => 'Novembro',
               '12' => 'Dezembro');

if($_REQUEST['act'] == 'gravar'){
    $detalhePlanejamento = new DetalhePlanejamento( $_REQUEST['dplid'] );

    $arCamposNulos = array();
    if($_REQUEST['uexid'] == ''){
        $_REQUEST['uexid'] = null;
        $arCamposNulos[]   = 'uexid';
    }

    if($_REQUEST['tppid'] == ''){
        $_REQUEST['tppid'] = null;
        $arCamposNulos[]   = 'tppid';
    }

    if($_REQUEST['esfid'] == ''){
        $_REQUEST['esfid'] = null;
        $arCamposNulos[]   = 'esfid';
    }

    if( !$_REQUEST['paiid'] ){
		$_REQUEST['paiid'] = 1; //valor do Brasil
    }else{
    	if($_REQUEST['paiid'] == ''){
       		$_REQUEST['paiid'] = null;
    	    $arCamposNulos[]   = 'paiid';
	    }
    }

    if($_REQUEST['estuf'] == ''){
        $_REQUEST['estuf'] = null;
        $arCamposNulos[]   = 'estuf';
    }

    if($_REQUEST['muncod'] == ''){
        $_REQUEST['muncod'] = null;
        $arCamposNulos[]   = 'muncod';
    }

	if ( $_REQUEST['estuf'] == 'DF' ){
		$codigoMunicipio = "5300108"; // Brasilia
		$codigoRegiaoAdministrativa = $_REQUEST['muncod'];
	}
	else{
		$codigoMunicipio = $_REQUEST['muncod'];
		$codigoRegiaoAdministrativa = null;
        $arCamposNulos[]   = 'rgaid';
	}
   
    if($_REQUEST['tpiid'] == ''){
        $_REQUEST['tpiid'] = null;
        $arCamposNulos[]   = 'tpiid';
    }
   
    if($_REQUEST['dplprioridade'] == ''){
        $_REQUEST['dplprioridade'] = null;
        $arCamposNulos[]   = 'dplprioridade';
    }
   
    if($_REQUEST['segid'] == ''){
        $_REQUEST['segid'] = null;
        $arCamposNulos[]   = 'segid';
    }
   
    if($_REQUEST['mpnid'] == ''){
        $_REQUEST['mpnid'] = null;
        $arCamposNulos[]   = 'mpnid';
    }
   
    if($_REQUEST['dplflgemendaparlamentar'] == 'f' || $_REQUEST['tauid'] == '' ){
        $_REQUEST['tauid'] = null;
        $arCamposNulos[]   = 'tauid';
    }
   
    if($_REQUEST['dplflgemendaparlamentar'] == 'f' || $_REQUEST['ppoid'] == ''){
        $_REQUEST['ppoid'] = null;
        $arCamposNulos[]   = 'ppoid';
    }
   
    if($_REQUEST['dplflgemendaparlamentar'] == 'f' || $_REQUEST['aueid'] == ''){
        $_REQUEST['aueid'] = null;
        $arCamposNulos[]   = 'aueid';
    }

    if($_REQUEST['dplflgcontribuippa'] == 't'){
        $_REQUEST['dplmetappaporque'] = '';
    }
    
	//recuperando o $acaid
	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}

    $arDados = array();
    $arDados = array( 'dplid'                   => (int) $_REQUEST['dplid'],
    				  'pplid'					=> (int) $_REQUEST['pplid'],
                      'uexid'          			=> $_REQUEST['uexid'],
                      'tppid'          			=> $_REQUEST['tppid'],
                      'dplprioridade'         	=> $_REQUEST['dplprioridade'],
                      'dpltitulo'          		=> $_REQUEST['dpltitulo'],
                      'dplobjetivo'          	=> $_REQUEST['dplobjetivo'],
                      'dpljustificativa'        => $_REQUEST['dpljustificativa'],
                      'dpldescricao'          	=> $_REQUEST['dpldescricao'],
                      'dploutrasiniciativas'	=> $_REQUEST['dploutrasiniciativas'],
                      'dplvlrcusteio'  			=> $_REQUEST['dplvlrcusteio'] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['dplvlrcusteio']) : $_REQUEST['dplvlrcusteio'],
                      'dplvlrcapital'  			=> $_REQUEST['dplvlrcapital'] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['dplvlrcapital']) : $_REQUEST['dplvlrcapital'],
                      'esfid'					=> $_REQUEST['esfid'],
                      'paiid'					=> $_REQUEST['paiid'],
                      'estuf'					=> $_REQUEST['estuf'],
                      'muncod'					=> $codigoMunicipio,
    				  'acaid'					=> $acaid ? $acaid : null,
                      'rgaid'					=> $codigoRegiaoAdministrativa,
    				  'dplprosecundariometa'	=> $_REQUEST['dplprosecundariometa'],
					  'pduid'					=> $_REQUEST['pduid'],
					  'udaid'					=> $_REQUEST['udaid'],
					  'ploid'					=> $_REQUEST['ploid'],
					  'ploflgappcs'			    => $_REQUEST['hdn_ploflgappcs'] ? $_REQUEST['hdn_ploflgappcs'] : 'f',
                      'tpiid'                   => $_REQUEST['tpiid'],
                      'segid'                   => $_REQUEST['segid'],
                      'mpnid'                   => $_REQUEST['mpnid'],
                      'dpldtinicio'             => formata_data_sql($_REQUEST['dpldtinicio']),
                      'dpldttermino'            => formata_data_sql($_REQUEST['dpldttermino']),
                      'dplflgemendaparlamentar' => $_REQUEST['dplflgemendaparlamentar'] ? $_REQUEST['dplflgemendaparlamentar'] : 'f',
                      'tauid'                   => $_REQUEST['tauid'],
                      'ppoid'                   => $_REQUEST['ppoid'],
                      'aueid'                   => $_REQUEST['aueid'],
                      'dplflgcontribuippa'      => $_REQUEST['dplflgcontribuippa'] ? $_REQUEST['dplflgcontribuippa'] : 't',
                      'dplmetappaporque'        => $_REQUEST['dplmetappaporque']
                    );

	if(empty($_REQUEST['dplid'])){
	   $arDados['docid']		= wf_cadastrarDocumento( WF_DETALHE_PLANEJAMENTO, 'Detalhamento Planejamento' );
	   $arDados['dplstatus']	= 'A';
       $arDados['usucpf']       = $_SESSION['usucpf'];
	   $arDados['prsano']		= $_SESSION['exercicio'];
	}else{
       //dependendo do estado atual ele grava os campos
       $dadosTemp   = $detalhePlanejamento->getDados();
       $estadoAtual = wf_pegarEstadoAtual($dadosTemp['docid']);
       
        if ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDOAPROVACAOPI){
            $arDados['dplnumeropi'] = $_REQUEST['dplnumeropi'];
        }elseif ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDODESCENTRALIZACAO){
            $arDados['dplnotacredito'] = $_REQUEST['dplnotacredito'];
        }
    }
    
    $dplid    = $detalhePlanejamento->popularDadosObjeto($arDados)->salvar(null, null, $arCamposNulos);

    $cFisico                = new CronogramaFisico();
    $cOrcamentario          = new CronogramaOrcamentario();
    $cFinanceiro            = new CronogramaFinanceiro();
    $projetoIni             = new ProjetoIniciativa();
    $projetoCon             = new ProjetoContratacao();
    $natDetalhamento        = new NaturezaDetalhamento();
    $metaPpaDetalhamento    = new MetaPpaDetalhamento();

    $cFisico->apagaPorDetalhe($dplid);
    $cOrcamentario->apagaPorDetalhe($dplid);
    $cFinanceiro->apagaPorDetalhe($dplid);
    $projetoIni->apagaPorDetalhe($dplid);
    $projetoCon->apagaPorDetalhe($dplid);
    $natDetalhamento->apagaPorDetalhe($dplid);
    $metaPpaDetalhamento->apagaPorDetalhe($dplid);

    $_REQUEST['dplid'] = $dplid;
    $_REQUEST['pplid'] = null;
    
    // gravando os cronogramas
    foreach ($arMes as $numMes => $nomeMes){
        if($_REQUEST["crfprevistomes{$numMes}"]){
            $_REQUEST["crfprevistomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["crfprevistomes{$numMes}"]);
        }else{
            $_REQUEST["crfprevistomes{$numMes}"] = null;
        }


        if($_REQUEST["croprevistocusteiomes{$numMes}"]){
            $_REQUEST["croprevistocusteiomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["croprevistocusteiomes{$numMes}"]);
        }else{
            $_REQUEST["croprevistocusteiomes{$numMes}"] = null;
        }
        if($_REQUEST["croprevistocapitalmes{$numMes}"]){
            $_REQUEST["croprevistocapitalmes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["croprevistocapitalmes{$numMes}"]);
        }else{
            $_REQUEST["croprevistocapitalmes{$numMes}"] = null;
        }


        if($_REQUEST["cfiprevistocusteiomes{$numMes}"]){
            $_REQUEST["cfiprevistocusteiomes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["cfiprevistocusteiomes{$numMes}"]);
        }else{
            $_REQUEST["cfiprevistocusteiomes{$numMes}"] = null;
        }
        if($_REQUEST["cfiprevistocapitalmes{$numMes}"]){
            $_REQUEST["cfiprevistocapitalmes{$numMes}"] = str_replace(array('.', ','), array('', '.'), $_REQUEST["cfiprevistocapitalmes{$numMes}"]);
        }else{
            $_REQUEST["cfiprevistocapitalmes{$numMes}"] = null;
        }
    }
    
    $cOrcamentario->popularDadosObjeto()->salvar();
    $cFinanceiro->popularDadosObjeto()->salvar();
    $cFisico->popularDadosObjeto()->salvar();

    // gravando as vinculações ao planejamento
    if( is_array( $_REQUEST['vinculacaoPlanejamento'] ) && !empty( $_REQUEST['vinculacaoPlanejamento'] ) ){
    	foreach( $_REQUEST['vinculacaoPlanejamento'] as $vinculacaoPlanejamento ){
			$dadosVinculacaoPlanejamento 		  = array();
			$dadosVinculacaoPlanejamento['dplid'] = (int) $dplid;
			$dadosVinculacaoPlanejamento['iteid'] = (int) $vinculacaoPlanejamento;

			$projetoIni->popularDadosObjeto($dadosVinculacaoPlanejamento)->salvar();
			$projetoIni->setDadosNull();

    	}
    }

    // grava as contratações
    if( (is_array( $_REQUEST['forma'] ) && !empty($_REQUEST['forma'])) && (is_array( $_REQUEST['objeto'] ) && !empty($_REQUEST['objeto'])) && (is_array( $_REQUEST['valorcusteio'] ) && !empty($_REQUEST['valorcapital'])) && (is_array( $_REQUEST['valorcapital'] ) && !empty($_REQUEST['valorcapital'])) ){
    	foreach( $_REQUEST['forma'] as $chave => $forma ){
			$dadosProjetoContratacao 		  			= array();
			$dadosProjetoContratacao['dplid'] 			= (int) $dplid;
			$dadosProjetoContratacao['itrid'] 			= (int) $forma;
			$dadosProjetoContratacao['prcnome']			= $_REQUEST['objeto'][$chave];
			$dadosProjetoContratacao['prcvalorcusteio']	= $_REQUEST['valorcusteio'][$chave] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['valorcusteio'][$chave]) : $_REQUEST['valorcusteio'][$chave];
			$dadosProjetoContratacao['prcvalorcapital']	= $_REQUEST['valorcapital'][$chave] ? str_replace(array('.', ','), array('', '.'), $_REQUEST['valorcapital'][$chave]) : $_REQUEST['valorcapital'][$chave];

			$projetoCon->popularDadosObjeto($dadosProjetoContratacao)->salvar();
			$projetoCon->setDadosNull();
    	}
    }
    
    
    // grava as metas PPA
    if($_REQUEST['dplflgcontribuippa'] == 't'){
        if( (is_array( $_REQUEST['metappa'] ) && !empty($_REQUEST['metappa'])) && (is_array( $_REQUEST['quantidade'] ) && !empty($_REQUEST['quantidade']))  ){
            foreach( $_REQUEST['metappa'] as $chave => $metappa ){
                $dadosProjetoContratacao                    = array();
                $dadosMetaPpaDetalhamento['mppid']          = (int) $metappa;
                $dadosMetaPpaDetalhamento['dplid'] 			= (int) $dplid;
                $dadosMetaPpaDetalhamento['mpdquantidade'] 	= (int) $_REQUEST['quantidade'][$chave];

                $metaPpaDetalhamento->popularDadosObjeto($dadosMetaPpaDetalhamento)->salvar();
                $metaPpaDetalhamento->setDadosNull();
            }
        }
    }

    // grava as naturezas de detalhamento
    $naturezaDespesa = is_array($_REQUEST['natureza']) ? $_REQUEST['natureza'] : array();

    foreach ( $naturezaDespesa as $k=>$naturezaOrcamento ){

        if ( $naturezaOrcamento != "" )
            {
            $sql = "SELECT ndpid FROM public.naturezadespesa WHERE ndpcod = '". str_pad($naturezaOrcamento,8, '0', STR_PAD_RIGHT) ."' ";
            $ndpid = $db->pegaUm($sql,0);

            $valor = str_replace( ',', '.', str_replace('.','',$_REQUEST['valor'][$k]) );

            $sql = sprintf( "INSERT INTO 
                                sisplan.naturezadetalhamento ( ndpid, dplid, nadvalor )
                            VALUES
                                ( %d, %d, %s)", $ndpid,
                                                $dplid,
                                                $valor );

            $db->executar( $sql );
        }
    }

	//INSERE ABRANGENCIA
	$sql = "DELETE FROM sisplan.projetoregionalizacao WHERE dplid = '". (int) $dplid ."'";
	$db->executar($sql);

	if( $_REQUEST['hdn_atisndetalharregionalizacao'] == 'true'){
		$abrpais = is_array($_REQUEST['pais']) ? $_REQUEST['pais'] : array();
		foreach ( $abrpais as $k=>$pais )
		{
			if ( $pais != "" )
			{
				$sql   = "SELECT paiid FROM territorios.pais WHERE paidescricao = '". $pais ."'";
				$paiid = $db->pegaUm($sql,0);

				$arrayuf = explode(" - ", $_REQUEST['uf'][$k]);
				$estuf   = $arrayuf[0];


				if ( $estuf == 'DF' )
				{
					$sql      = "select rgaid from planointerno.regiaoadministrativa where rgadsc = '". $_REQUEST['mun'][$k] ."'";
					$rgaid    = $db->pegaUm($sql,0);
					$estufmun = '';
					$muncod   = '';
				}
				else
				{
					$arraymun = explode(" - ", $_REQUEST['mun'][$k]);
					$sql      = "select muncod from territorios.municipio where estuf = '". $arraymun[0] ."' and mundescricao = '". $arraymun[1] ."'";
					$muncod   = $db->pegaUm($sql);
				}

				$sql = sprintf(
					"INSERT INTO sisplan.projetoregionalizacao
						( dplid, paiid, estuf, muncod, rgaid )
					 VALUES
						( %s, %s, %s, %s, %s )",
					$dplid,
					$paiid,
					$estuf ? "'".$estuf."'" : 'NULL',
					$muncod ? "'".$muncod."'" : 'NULL',
					$rgaid ? $rgaid : 'NULL' );

				$db->executar( $sql );
			}
		}
	}

    $db->commit();
    $db->sucesso( 'principal/consultarPlanejamento');
    die();
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

$unidadeExecutora	= new UnidadeExecutora();
$responsabilidades	= $unidadeExecutora->getResponsabilidadeUnidadesByUsuario();

$excecao 	= new ExcecaoUnidade();
$excecoes	= $excecao->verificaExcecao(null, true);

$modeloDetalhe = new DetalhePlanejamento();
$dplid         = $modeloDetalhe->getIdByPlanejamento($_REQUEST['pplid']);

if($dplid){ //se já existir detalhamento

    $modelo = new DetalhePlanejamento($dplid);

    $dados = $modelo->getDados();

    if ( !$dados['docid']){
    	$arDados 		  = array();
		$arDados['docid'] = wf_cadastrarDocumento( WF_DETALHE_PLANEJAMENTO, 'Detalhamento Planejamento' );

    	$modelo->popularDadosObjeto($arDados)->salvar();
    	$modelo->commit();
    }

    extract($modelo->getDados());

    $estadoAtual = wf_pegarEstadoAtual($docid);
    
    if ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDOCORRECAO){
        $permissao_formulario           = 'S';
        $permissao_formulario_detalhe   = 'S';
    }else{
        $permissao_formulario           = 'N';
        $permissao_formulario_detalhe   = 'S';
    }
    
        
    $dplid 				= (int)$dplid;
	$_REQUEST['dplid']	= $dplid;
    
    $muncod = !empty($rgaid) ? $rgaid : $muncod;

    $vlrtotal      = formata_valor( $dplvlrcusteio + $dplvlrcapital );
    $dplvlrcusteio = formata_valor($dplvlrcusteio);
    $dplvlrcapital = formata_valor($dplvlrcapital);
    

    $cFisico       = new CronogramaFisico();
    $cOrcamentario = new CronogramaOrcamentario();
    $cFinanceiro   = new CronogramaFinanceiro();

    $dadosFisico       = $cFisico->pegaPorDetalhe($dplid);
    $dadosOrcamentario = $cOrcamentario->pegaPorDetalhe($dplid);
    $dadosFinanceiro   = $cFinanceiro->pegaPorDetalhe($dplid);

    if($dadosFisico){
        foreach ($dadosFisico as $k => $value){
            if(($k != 'crfid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosFisico[$k] = intVal($value);
        }
        extract($dadosFisico);
    }


    if($dadosOrcamentario){
        foreach ($dadosOrcamentario as $k => $value){
            if(($k != 'croid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosOrcamentario[$k] = formata_valor($value);
        }
        extract($dadosOrcamentario);
    }

    if($dadosFinanceiro){
        foreach ($dadosFinanceiro as $k => $value){
            if(($k != 'cfiid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosFinanceiro[$k] = formata_valor($value);
        }
        extract($dadosFinanceiro);
    }

}else{ //não foi gravado um detalhamento ainda
   
    $modelo = new ProjetoPlanejamento($_REQUEST['pplid']);

    $dados = $modelo->getDados();

    extract($modelo->getDados());
    
    $estadoAtualPlanejamento = wf_pegarEstadoAtual($dados['docid'] );    
    
    //redireciona para atela de lista se o estado workflow do planejamento não estiver aprovado
    if( $estadoAtualPlanejamento['esdid'] != WF__PLANEJAMENTO_ESTADO_APROVADO ) {
        die("<script> 
                alert('Não é possível detalhar esse registro porque o planejamento não se encontra aprovado.');
                window.location='?modulo=principal/consultarPlanejamento&acao=".$_REQUEST['acao']."'; 
             </script>");
    }
    
    $pplid 				= (int)$pplid;
	$_REQUEST['pplid']	= $pplid;
    
    $muncod = !empty($rgaid) ? $rgaid : $muncod;

    $dplvlrcusteio  = $dplvlrcusteio ? $dplvlrcusteio : $pplvlrcusteio;
    $dplvlrcapital  = $dplvlrcapital ? $dplvlrcapital : $pplvlrcapital;
    $vlrtotal       = $dplvlrcusteio + $dplvlrcapital;
    $dplvlrcusteio  = formata_valor( $dplvlrcusteio );
    $dplvlrcapital  = formata_valor( $dplvlrcapital );
    $vlrtotal       = formata_valor( $vlrtotal );
    
    $cFisico       = new CronogramaFisico();
    $cOrcamentario = new CronogramaOrcamentario();
    $cFinanceiro   = new CronogramaFinanceiro();

    $dadosFisico       = $cFisico->pegaPorPlanejamento($pplid);
    $dadosOrcamentario = $cOrcamentario->pegaPorPlanejamento($pplid);
    $dadosFinanceiro   = $cFinanceiro->pegaPorPlanejamento($pplid);

    if($dadosFisico){
        foreach ($dadosFisico as $k => $value){
            if(($k != 'crfid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosFisico[$k] = intVal($value);
        }
        extract($dadosFisico);
    }


    if($dadosOrcamentario){
        foreach ($dadosOrcamentario as $k => $value){
            if(($k != 'croid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosOrcamentario[$k] = formata_valor($value);
        }
        extract($dadosOrcamentario);
    }

    if($dadosFinanceiro){
        foreach ($dadosFinanceiro as $k => $value){
            if(($k != 'cfiid' && $k != 'pplid' && $k != 'dplid') && !is_null($value) )
                $dadosFinanceiro[$k] = formata_valor($value);
        }
        extract($dadosFinanceiro);
    }
    
}

/*
 * Aqui ficam as verificações e recuperação de dados que independem se existe ou
 * não o detalhamento ainda
 */

//recuperando dados da ação
if ($acaid){
    $dados_acaid = $db->pegaLinha( sprintf( "SELECT a.prgcod, a.acacod, a.unicod, a.loccod, u.unmdsc, p.prodsc FROM monitora.acao a LEFT JOIN public.produto
    p ON p.procod = a.procod LEFT JOIN public.unidademedida u ON u.unmcod = a.unmcod
    WHERE acaid = '%s'", $acaid ) );

    extract($dados_acaid);
}

//verificando as responsabilidades para bloquear a tela
if( !$db->testa_superuser() &&
    !possuiPerfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO) &&
    !possuiPerfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) &&
    !possuiPerfil(PERFIL_COORDENADOR_ORCAMENTO) &&
    !possuiPerfil(PERFIL_COORDENADOR_PLANEJAMENTO) ){
    if( is_array( $responsabilidades ) &&  !empty( $responsabilidades) ){
        if ( in($uexid, $responsabilidades) ){
            $permissao_formulario           = 'S';
            $permissao_formulario_detalhe   = 'S';
        }else{
            $permissao_formulario           = 'N';
            $permissao_formulario_detalhe   = 'N';
        }
    }else{
        $permissao_formulario           = 'N';
        $permissao_formulario_detalhe   = 'N';
    }
}
    
//Verificando a data de inicio e termino do planejamento ( definida na programacaoexercicio )
$programacaoExercicio   = new ProgramacaoExercicio();
$permissao              = $programacaoExercicio->verificaPermissaoPlanejamento();

if( !$permissao ){
    //verifica a liberação de exceção
    $excecaoUnidade 	= new ExcecaoUnidade();
    $excecao			= $excecaoUnidade->verificaExcecao($uexid);

    if( !$excecao ){
        $permissao_formulario               = 'N';
        $permissao_formulario_detalhe       = 'N';
    }else{
        $permissao_formulario               = 'S';
        $permissao_formulario_detalhe       = 'S';
    }
}



monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form action="" method="post" name="formulario">
    <input type="hidden" id="pplid" name="pplid" value="<?= $pplid; ?>"/>
    <input type="hidden" id="dplid" name="dplid" value="<?= $dplid; ?>"/>
    <input type="hidden" name="act" id="act" value="gravar" />
    <input type="hidden" name="hdn_esfid" id="hdn_esfid" value="<?php echo $esfid; ?>" />
    <input type="hidden" name="hdn_paiid" id="hdn_paiid" value="<?php echo $paiid; ?>" />
    <input type="hidden" name="hdn_estuf" id="hdn_estuf" value="<?php echo $estuf; ?>" />
    <input type="hidden" name="hdn_muncod" id="hdn_muncod" value="<?php echo $muncod; ?>" />
    <input type="hidden" name="hdn_ploflgappcs" id="hdn_ploflgappcs" value="<?php echo $ploflgappcs; ?>" />
    <input type="hidden" name="hdn_atisndetalharregionalizacao" id="hdn_atisndetalharregionalizacao" value="" />

    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Executora:
            </td>
            <td>
                <?php
                    $modelo = new UnidadeExecutora();
					$dados  = $modelo->recuperarTodos("uexid AS codigo, uexsigla || ' - ' || uexdsc AS descricao", array("uexstatus = 'A'"," uexid = ". (int) $uexid ), "descricao");
                    $db->monta_combo("uexid", $dados, 'N', '&nbsp', '', '', '', '517', 'S', 'uexid');
                ?>
            </td>
            <td rowspan="7" colspan="2">
                <?php
                    if( $dplid && $permissao_formulario_detalhe == 'S' ){
                        wf_desenhaBarraNavegacao( $docid, array("dplid" => $dplid) );
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Tipo de Projeto:
            </td>
            <td>
                <?php
                    $modelo = new TipoProjetoPlanejamento();
					$dados  = $modelo->recuperarTodos("tppid AS codigo, tppnome || ' - ' || tppdescricao AS descricao", array("tppstatus = 'A'"), "descricao");
                    $db->monta_combo("tppid", $dados, 'N', '&nbsp', '', '', '', '517', 'S', 'tppid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Prioridade:
            </td>
            <td colspan="5">
                 <? 
                    $dplprioridade = $dplprioridade ? $dplprioridade : $pplprioridade ;
                    echo campo_texto('dplprioridade', 'S', 'N', '', 10, 4, '####', '','','','','id="dplprioridade"','','',''); 
                 ?>
            </td>
        </tr>
        <?php
        if($dplid){
            $estadoAtual = wf_pegarEstadoAtual($docid);
            
            if ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDOAPROVACAOPI){
                ?>
                     <tr>
                        <td align='right' class="SubTituloDireita" style="width:25%;">
                             Número do PI:
                        </td>
                        <td colspan="5">
                             <?php 
                                echo campo_texto('dplnumeropi', 'S', 'S', '', 25, 11, '', '','','','','id="dplnumeropi"','','',''); 
                             ?>
                        </td>
                    </tr>
                <?php
            }elseif ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDODESCENTRALIZACAO){
                ?>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="width:25%;">
                             Número da nota de crédito:
                        </td>
                        <td colspan="5">
                             <?php 
                                echo campo_texto('dplnotacredito', 'S', 'S', '', 25, 11, '###########', '','','','','id="dplnotacredito"','','',''); 
                             ?>
                        </td>
                    </tr>
                <?php
            }
        }
        ?>    
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Título (Definição do Objeto):
		    </td>
            <td>
                <?php
                    $dpltitulo = $dpltitulo ? $dpltitulo : $ppltitulo ;
                    echo campo_textarea( 'dpltitulo', 'S', 'N', '', 95, 3, 200 ); 
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Objetivos (Para que?):
		    </td>
            <td>
                <?php
                    $dplobjetivo = $dplobjetivo ? $dplobjetivo : $pplobjetivo ;
                    echo campo_textarea( 'dplobjetivo', 'S', 'N', '', 95, 5, 2000 ); 
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Justificativa (Por que?):
		    </td>
            <td>
                <?php
                    $dpljustificativa = $dpljustificativa ? $dpljustificativa : $ppljustificativa ;
                    echo campo_textarea( 'dpljustificativa', 'S', 'N', '', 95, 5, 2000 ); 
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Descrição das Atividades (Como?):
		    </td>
            <td>
                <?php
                    $dpldescricao = $dpldescricao ? $dpldescricao : $ppldescricao ;
                    echo campo_textarea( 'dpldescricao', 'S', 'N', '', 95, 5, 2000 ); 
                ?>
            </td>
		</tr>
		<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Vinculação ao Planejamento Estratégico:</td>
			<td>
				<?php
					if( $dplid )
					{
						$sql_vinculacaoPlanejamento = " select
															ite.iteid as codigo,
															dfe.dfedsc || ' - ' || ite.itedsc as descricao,
															dfeobservacao as titulo
														from
															sisplan.iniciativaestrategica ite
														inner join
															sisplan.projetoiniciativa pin on pin.iteid = ite.iteid
														inner join
															sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
														where
															ite.itestatus = 'A' and
															pin.dplid = '" . (int) $dplid . "'
														order by
															ite.iteid";

						$vinculacaoPlanejamento = $db->carregar( $sql_vinculacaoPlanejamento );
						$vinculacaoPlanejamento = $vinculacaoPlanejamento ? $vinculacaoPlanejamento : array();
					}else{
                        $sql_vinculacaoPlanejamento = " select
                                                            ite.iteid as codigo,
                                                            dfe.dfedsc || ' - ' || ite.itedsc as descricao,
                                                            dfeobservacao as titulo
                                                        from
                                                            sisplan.iniciativaestrategica ite
                                                        inner join
                                                            sisplan.projetoiniciativa pin on pin.iteid = ite.iteid
                                                        inner join
                                                            sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
                                                        where
                                                            ite.itestatus = 'A' and
                                                            pin.pplid = '" . (int) $pplid . "'
                                                        order by
                                                            ite.iteid";

						$vinculacaoPlanejamento = $db->carregar( $sql_vinculacaoPlanejamento );
						$vinculacaoPlanejamento = $vinculacaoPlanejamento ? $vinculacaoPlanejamento : array();
                    }

						$sql_combo = " select
											ite.iteid as codigo,
											dfe.dfedsc || ' - ' || ite.itedsc as descricao,
											dfeobservacao as titulo
										from
											sisplan.iniciativaestrategica ite
										inner join
											sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
										where
											ite.itestatus = 'A'
										order by
											ite.iteid";

					combo_popup(
						'vinculacaoPlanejamento',
						$sql_combo,
						'Selecione a(s) Iniciativa(s) Estratégica(s)',
						'400x400',
						0,
						array(),
						'N',
						'N',
						true
					);
					echo obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Outras Iniciativas:
		    </td>
            <td>
                <?
                    $dploutrasiniciativas = $dploutrasiniciativas ? $dploutrasiniciativas : $pploutrasiniciativas;
                    echo campo_textarea( 'dploutrasiniciativas', 'N', $permissao_formulario, '', 95, 5, 2000 ); 
                ?>
            </td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Tipo de Instrumento:
            </td>
            <td colspan="5">
                <?php
                    $modelo = new TipoInstrumento();
                    $dados  = $modelo->recuperarTodos("tpiid AS codigo, tpidsc AS descricao", array("tpistatus = 'A'"), "descricao");
                    $db->monta_combo("tpiid", $dados, $permissao_formulario_detalhe, '&nbsp', '', '', '', '520', 'S', 'tpiid');
                ?>
            </td>
        </tr>        
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Segmento Cultural:
            </td>
            <td colspan="5">
                <?php
                    $modelo = new Segmento();
                    $dados  = $modelo->recuperarTodos("segid AS codigo, segdsc AS descricao", array("segstatus = 'A'"), "descricao");
                    $db->monta_combo("segid", $dados, $permissao_formulario_detalhe, '&nbsp', '', '', '', '520', 'S', 'segid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Meta PNC:
            </td>
            <td colspan="5">
                <?php
                    $modelo = new MetaPnc();
                    $dados  = $modelo->recuperarTodos("mpnid AS codigo, mpnnome AS descricao", array("mpnstatus = 'A'", "prsano = '{$_SESSION['exercicio']}'"), "descricao");
                    $db->monta_combo("mpnid", $dados, $permissao_formulario_detalhe, '&nbsp', '', '', '', '520', 'S', 'mpnid');
                ?>
            </td>
        </tr>
        <tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Data de Início:
		    </td>
            <td>
                <?= campo_data2('dpldtinicio', 'S', $permissao_formulario_detalhe, '', 'S'); ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Data de Término:
		    </td>
            <td>
                <?= campo_data2('dpldttermino', 'S', $permissao_formulario_detalhe, '', 'S'); ?>
            </td>
		</tr>
        <tr>
			<td colspan="8" class="SubTituloCentro" style="background-color: #DCDCDC">Meta PPA:</td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                  Contribui para alguma Meta PPA?:
            </td>
            <td colspan="5">
				<input type="radio" onclick="mudaMetaPpa('true');" name="dplflgcontribuippa" id="dplflgcontribuippa-t" <?php echo ( $dplflgcontribuippa == 't' || empty($dplflgcontribuippa)  ) ? ' checked="checked" ' : "" ?> value="t" /> <label for="dplflgcontribuippa-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" onclick="mudaMetaPpa('false');" name="dplflgcontribuippa" id="dplflgcontribuippa-f" <?php echo $dplflgcontribuippa == 'f'  ? ' checked="checked" ' : "" ?> value="f" /> <label for="dplflgcontribuippa-f" style="cursor:pointer;"> Não</label>
            </td>
        </tr>
        <tr class="MetaPpaTrue">
            <td align='right' class="SubTituloDireita" style="width:25%;"></td>
            <td>
                <table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
                   <thead>
                       <tr>
                           <td width="55%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Meta PPA</strong></td>
                           <td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;""  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Quantidade</strong></td>
                           <td width="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                       </tr>
                   </thead>
                       <tr bgcolor="#f7f7f7">
                           <td style="white-space: nowrap;">
                               <?php 
                                    if( $dplid ){
                                        $modelo = new MetaPpaDetalhamento();
                                        $dados  = $modelo->getComboMetaPpa($dplid);
                                    }else{
                                        $modelo = new MetaPpa();
                                        $dados  = $modelo->recuperarTodos("mppid AS codigo, mppnome AS descricao", array("mppstatus = 'A'", "prsano = '{$_SESSION['exercicio']}'"), "descricao");
                                    }
                                    
                                    $db->monta_combo("novo_mppid", $dados, $permissao_formulario_detalhe, '&nbsp', '', '', '', '300', 'S', 'novo_mppid');
                               ?>
                           </td>
                           <td>
                               <?= campo_texto( 'nova_quantidade', $permissao_formulario_detalhe, 'S', '', 15, 7, '#######', '', 'right', '', 0, ' id="nova_quantidade" onblur="MouseBlur(this);" ' ); ?>
                           </td>
                           <td align="left">
                               <?php if ($permissao_formulario_detalhe == 'S'){ ?>
                               <a href="javascript:metappa_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
                                   <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
                               </a>
                               <?php }else{ ?>
                               <a title="Incluir" style="margin: 0; padding: 0 6px;">
                                   <img src="../imagens/gif_inclui_d.gif" border="0" align="absmiddle"/>
                               </a>
                               <?php } ?>
                           </td>
                       </tr>
                   <tr>
                       <td colspan="3">
                           <div class="div_rolagem" id="div_tabela_metappa" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
                               <table id="tabela_metappa" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
                                   <tr valign="bottom">
                                       <td width="50%" style="height: 0; margin: 0; padding: 0;"></td>
                                       <td width="40%" style="height: 0; margin: 0; padding: 0;"></td>
                                       <td width="10%" style="height: 0; margin: 0; padding: 0;"></td>
                                   </tr>
                                   <?php
                                        if( $dplid ){
                                            $metaPpaDetalhamento = new MetaPpaDetalhamento();
                                            $metasPpa            = $dplid ? $metaPpaDetalhamento->getMetasPpaByDetalhe( $dplid ) : array();

                                            foreach( $metasPpa AS $chave => $metappa ){ 
                                             ?>
                                                <tr id="metappa_<?php echo $chave + 1; ?>">
                                                    <td width="'50%" style="text-align:left;">
                                                        <?php echo $metappa['mppnome']; ?>
                                                        <input id="metappa_<?php echo $chave + 1; ?>_valor" type="hidden" name="metappa[]" value="<?php echo $metappa['mppid']; ?>">
                                                    </td>
                                                    <td width="'30%" style="text-align:right;">
                                                        <?php echo $metappa['mpdquantidade']; ?>
                                                        <input type="hidden" name="quantidade[]" value="<?php echo $metappa['mpdquantidade']; ?>">
                                                    </td>
                                                    <td width="'20%" style="text-align:center;">
                                                        <?php if ( $permissao_formulario_detalhe == 'S' ){ ?>
                                                            <a href="javascript:metappa_alterar('metappa_<?php echo $chave + 1; ?>', '<?php echo $metappa['mppid']; ?>', '<?php echo $metappa['mppnome']; ?>', '<?php echo $metappa['mpdquantidade']; ?>')"> 
                                                                <img src="../imagens/alterar.gif" border="0" align="absmiddle"/>
                                                            </a>
                                                            <a href="javascript:metappa_remover('metappa_<?php echo $chave + 1; ?>', '<?php echo $metappa['mppid']; ?>', '<?php echo $metappa['mppnome']; ?>', 'true')"> 
                                                                <img src="../imagens/excluir.gif" border="0" align="absmiddle"/>
                                                            </a>
                                                       <?php }else{ ?>
                                                            <a><img src="../imagens/alterar_01.gif" border="0" align="absmiddle"/></a>
                                                            <a><img src="../imagens/excluir_01.gif" border="0" align="absmiddle"/></a>
                                                       <?php } ?>
                                                    </td>
                                                </tr>
                                             <?
                                            }
                                        }
                                   ?>
                               </table>
                           </div>
                       </td>
                   </tr>
               </table>
            </td>
        </tr>
        <tr class="MetaPpaFalse">
            <td align='right' class="SubTituloDireita" style="width:25%;">Porque?</td>
            <td colspan="5">
                 <?php
                    echo campo_textarea( 'dplmetappaporque', 'S', $permissao_formulario_detalhe, '', 95, 5, 2000 ); 
                ?>
            </td>
        </tr>
        <tr>
			<td colspan="8" class="SubTituloCentro" style="background-color: #DCDCDC">Emenda:</td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                  Emenda parlamentar:
            </td>
            <td colspan="5">
				<input type="radio" onclick="mudaEmendaParlamentar('show');" name="dplflgemendaparlamentar" id="dplflgemendaparlamentar-t" <?php echo $dplflgemendaparlamentar == 't'  ? ' checked="checked" ' : "" ?> value="t" /> <label for="dplflgemendaparlamentar-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" onclick="mudaEmendaParlamentar('hide');" name="dplflgemendaparlamentar" id="dplflgemendaparlamentar-f" <?php echo ( $dplflgemendaparlamentar == 'f' || empty($dplflgemendaparlamentar)  ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="dplflgemendaparlamentar-f" style="cursor:pointer;"> Não</label>
            </td>
        </tr>
        <tr class="mudaEmendaParlamentar">
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Tipo de autor:
            </td>
            <td colspan="5">
                 <?php
                    $modelo = new TipoAutor();
                    $dados  = $modelo->recuperarTodos("tauid AS codigo, taudsc AS descricao", array("taustatus = 'A'"), "descricao");
                    $db->monta_combo("tauid", $dados, $permissao_formulario_detalhe, '&nbsp', 'carregaPartido();', '', '', '440', 'S', 'tauid');
                ?>
            </td>
        </tr>
        <tr class="mudaEmendaParlamentar">
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Partido:
            </td>
            <td colspan="5">
            	<div id="divPartido">
                 <?php
                      $modelo = new PartidoPolitico();
                      $dados  = $modelo->recuperarTodos("ppoid AS codigo, pposigla AS descricao", array("ppostatus = 'A'"), "descricao");
                      $db->monta_combo("ppoid", $dados, $permissao_formulario_detalhe, '&nbsp', 'carregaAutor();', '', '', '440', 'S', 'ppoid');
                ?>
            	</div>
            </td>
        </tr>
        <tr class="mudaEmendaParlamentar">
            <td align='right' class="SubTituloDireita" style="width:25%;">
                 Autor:
            </td>
            <td colspan="5">
            	<div id="divAutor">
                 <?php
                    $modelo = new AutorEmenda();
                    $dados  = $modelo->recuperarTodos("aueid AS codigo, auedsc AS descricao", array("auestatus = 'A'"), "descricao");
                    $db->monta_combo("aueid", $dados, $permissao_formulario_detalhe, '&nbsp', '', '', '', '440', 'S', 'aueid');
                ?>
            	</div>
            </td>
        </tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Pac:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Ações presentes nos APPCs <br>(Acordos de preservação do patrimônio cultural):
		    </td>
	        <td>
				<input type="radio" name="ploflgappcs" <?= $permissao_formulario == 'S' ? '' : 'disabled="disabled"'; ?> id="ploflgappcs-t" <?php echo $ploflgappcs == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="ploflgappcs-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" name="ploflgappcs" <?= $permissao_formulario == 'S' ? '' : 'disabled="disabled"'; ?> id="ploflgappcs-f" <?php echo ($ploflgappcs == 'f' || empty($ploflgappcs) ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="ploflgappcs-f" style="cursor:pointer;"> Não</label>
	        	<?php echo obrigatorio(); ?>
	        </td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Regionalização:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Localização da ação:</td>
			<td>
				<?php
					$sql = "SELECT esfid AS codigo, esfdsc AS descricao FROM planointerno.esfera";
					$db->monta_combo( "esfid",$sql,$permissao_formulario,"&nbsp;",'action_esfera','', '', '200', 'S', "esfid" );
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">País:</td>
			<td>
				<?php
					$sql = "SELECT paiid AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2";
					$db->monta_combo("paiid",$sql,$permissao_formulario,'&nbsp','','','','200','','paiid');
					//echo  obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<div id="estuf_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==''){ echo "display:none"; }else{ echo "display:block";  } ?> ">
				<?php
					$sql = "SELECT estuf AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2";
					$db->monta_combo("estuf",$sql,$permissao_formulario,'&nbsp','listar_municipios','','','','','estuf');
					//echo  obrigatorio();
				?>
				</div>
				<div id="estuf_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==2 || $atividade['esfid']==''){ ?>display:none;<?php } else { echo "display:block;"; } ?>">
					<?
						if ( $atividade['estuf'] == 'DF' )
						{
							$sql = "SELECT rgaid, rgaid as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
						}
						else
						{
							$sql = "SELECT estuf, muncod as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
						}
						$db->monta_combo("muncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','muncod');
					?>
				</div>
				<div id="muncod_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Detalhar regionalização:</td>
			<td>
				<input type="checkbox" <?= $permissao_formulario == 'S' ? '' : ' disabled="disabled" '; ?> style="cursor:pointer;" onclick="exibeAbrangencia();" id="atisndetalharregionalizacao" name="atisndetalharregionalizacao" value="true" <?php if($atisndetalharregionalizacao=="t"){ echo " checked "; } ?>/> <label for="atisndetalharregionalizacao" style="cursor:pointer;">Marque esta opção para informar múltiplos valores de regionalização.</label>
			</td>
		</tr>
		<tr id="trabrangencia" style="display:none">
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Abrangência:</td>
			<td>
				<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>País</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade Federativa</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
							<td width="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						</tr>
					</thead>
						<tr bgcolor="#f7f7f7">
							<td>
								<div id="pais_on">
									<? $sql = "SELECT paidescricao AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2"; ?>
									<? $db->monta_combo("abrpaiid",$sql,$permissao_formulario,'&nbsp','','','',200,'','abrpaiid'); ?>
								</div>
								<div id="pais_off"></div>
							</td>
							<td>
								<div id="abrestuf_on" >
									<? $sql = "SELECT estuf||' - '||estdescricao AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2"; ?>
									<? $db->monta_combo("abrestuf",$sql,$permissao_formulario,'&nbsp','listar_municipios_abrangencia','','',200,'','abrestuf'); ?>
								</div>
								<div id="abrestuf_off"></div>
							</td>
							<td align="right">
								<div id="abrmuncod_on" >
									<?
									//dbg($atividade['estuf']);
										if ( $atividade['abrestuf'] == 'DF' )
										{
											$sql = "SELECT rgaid, rgadsc as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
										}
										else
										{
											$sql = "SELECT estuf, estuf || ' - ' || mundescricao as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
										}
										$db->monta_combo("abrmuncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','abrmuncod');
									?>
								</div>
								<div id="abrmuncod_off"></div>
							</td>
							<td align="left">
                                <?php if ( $permissao_formulario == 'S' ){ ?>
                                    <a href="javascript:item_abrangencia_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
                                        <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
                                    </a>
                                <?php }else{ ?>
                                    <a title="Incluir" style="margin: 0; padding: 0 6px;">
                                        <img src="../imagens/gif_inclui_d.gif" border="0" align="absmiddle"/>
                                    </a>
                                <?php } ?>
							</td>
						</tr>
					<tr>
						<td colspan="4">
							<div class="div_rolagem" id="div_tabela_abrangencia" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
								<table id="tabela_abrangencia" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
									<tr valign="bottom">
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="32%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">PPA:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">PTRES:</td>
			<td>
				<?=campo_texto('ptres','N',$permissao_formulario,'',12,6,'######','','','Informe o número PTRES.','','id="ptres"');?>
				<span id="span_ptres"></span>
				<script type="text/javascript">
				$("ptres").observe("blur", function(event) {
				listar_ptres($("ptres").value);});
				</script>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
			<td><?php
				$sql = sprintf( "SELECT distinct p.prgcod as codigo, p.prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa p INNER JOIN monitora.acao a ON a.prgid = p.prgid WHERE a.unicod = '%s' and p.prgano = '%d' AND prgstatus = 'A' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				$db->monta_combo("prgcod",$sql,$permissao_formulario,'&nbsp','listar_acoes','','','570','S','prgcod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
			<td><?php

				$sql = array();
				if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
				{
					$sql = "
                    SELECT
                        ur.acacod
                    FROM sisplan.usuarioresponsabilidade ur
                    WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
                      AND ur.acacod is not null
                      AND ur.rpustatus = 'A'";

                    $acacodArr = (array) $db->carregarColuna($sql);
                    if ($acacodArr[0]){
                        $acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
                        $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acasnfinalistica IS TRUE AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
                    }
				}
				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_TECNICO_DEPARTAMENTO ) ||
					 usuario_possui_perfil( PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA ) ||
					 usuario_possui_perfil( PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao a WHERE acasnfinalistica IS TRUE AND unicod = '%s' and acasnrap = false AND prgano = '%d' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				}
				$db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','listar_unidades','','','570','S','acacod');
				//echo  obrigatorio();
			?><span id="span_acacod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Orçamentária:</td>
			<td><?php

				if ( $acacod )
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' and a.acacod = '%s' order by 2", $_SESSION['exercicio'], $acacod );
				else
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' order by 2", $_SESSION['exercicio'] );
				$db->monta_combo("unicod",$sql,$permissao_formulario,'&nbsp','listar_localizadores','','','570','S','unicod');
				//echo  obrigatorio();
			?><span id="span_unicod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
			<td><?php
					if ( $prgcod && $acacod && $unicod )
					$sql = sprintf( "SELECT DISTINCT a.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod WHERE a.prgano = '%d' AND a.unicod = '%s' AND a.prgcod = '%s' AND a.acacod = '%s' ORDER BY 2", $_SESSION['exercicio'], $unicod, $prgcod, $acacod );
				else
					$sql = "SELECT DISTINCT l.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod ORDER BY 2";
//					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','listar_produto','','','580','S','loccod');
					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','verificaPo','','','570','S','loccod');
			?><span id="span_loccod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Plano Orçamentário:</td>
			<td id="tdPlanoOrcamentario">
				<?php
                    $modelo = new PlanoOrcamentario();

					if($prgcod && $acacod && $unicod && $loccod){
						$dados  = $modelo->recuperarTodos("ploid AS codigo, plocod || ' - ' || plonome AS descricao", array("plostatus = 'A'"), "descricao");
						$permissao_formulario_po = 'S';
					}else{
						$dados = array(  array('codigo' => 0, 'descricao' => '') );
						$permissao_formulario_po = 'N';
					}

					$permissao_formulario_po = $permissao_formulario == 'S' ? $permissao_formulario_po : 'N';

                    $db->monta_combo("ploid", $dados, $permissao_formulario_po, '&nbsp', '', '', '', '570', 'S', 'ploid');
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Produto Secundário:</td>
			<td>
				<table width="570" border="0" cellpadding="0" cellspacing="2">
					<tr>
						<td width="230" class="SubTituloCentro" style="background-color: #DCDCDC">Produto</td>
						<td width="220" class="SubTituloCentro" style="background-color: #DCDCDC">Unidade Medida</td>
						<td width="120" class="SubTituloCentro" style="background-color: #DCDCDC">Meta</td>
					</tr>
					<tr>
						<td style="text-align:center"><?php
							$sql = "SELECT pduid AS codigo, pdudsc AS descricao FROM planointerno.produto WHERE pdustatus = 'A'";
							$db->monta_combo( "pduid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "pduid" );
						?></td>
						<td style="text-align:center"><?php
							$sql = "SELECT udaid AS codigo, udadsc AS descricao FROM planointerno.unidademedida WHERE udastatus = 'A'";
							$db->monta_combo( "udaid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "udaid" );
						?></td>
						<td style="text-align:center">
                            <? 
                                $dplprosecundariometa = $dplprosecundariometa ? $dplprosecundariometa : $pplprosecundariometa;
                                echo campo_texto('dplprosecundariometa','S',$permissao_formulario,'',12,10,'###.###.###','','','Informe a quantidade a ser executada.','','id="dplprosecundariometa"'); 
                            ?>
                        </td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Valor Previsto (anual):</td>
		</tr>
		<?php
			$style = '';
		?>
        <tr id="trCusteioCapital" style="<?= $style; ?>">
            <td align='right' class="SubTituloDireita">
                 &nbsp;
            </td>
            <td colspan="5">
                <input type="hidden" id="pmuid" name="pmuid" value="<?= $pmuid; ?>"/>
                <input type="hidden" id="saldoAutorizadoCusteio" name="saldoAutorizadoCusteio" value="<?= $saldoAutorizadoCusteio; ?>" />
                <input type="hidden" id="saldoAutorizadoCapital" name="saldoAutorizadoCapital" value="<?= $saldoAutorizadoCapital; ?>" />
                <input type="hidden" id="saldoDisponivelCusteio" name="saldoDisponivelCusteio" value="<?= $saldoDisponivelCusteio; ?>" />
                <input type="hidden" id="saldoDisponivelCapital" name="saldoDisponivelCapital" value="<?= $saldoDisponivelCapital; ?>" />
                <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 570px; float:left;">
                    <tr>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Custeio</b>
                        </td>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Capital</b>
                        </td>
                        <td class="SubTituloDireita" style="width:510px; text-align: center;">
                            <b>Total</b>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <?= campo_texto('dplvlrcusteio', 'S', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','','id="dplvlrcusteio"','calculaTotalPrevisto()','',''); ?>
                        </td>
                        <td>
                            <?= campo_texto('dplvlrcapital', 'S', $permissao_formulario, '', 22, 22, '#.###.###.###.###,##', '','','','','id="dplvlrcapital"','calculaTotalPrevisto()','',''); ?>
                        </td>
                        <td>
                            <?php
//                            	$total = ( str_replace(array('.', ','), array('', '.'), $pplvlrcusteio) ) + ( str_replace(array('.', ','), array('', '.'), $pplvlrcapital) );
                            	echo campo_texto('vlrtotal', 'N', 'N', '', 22, 22, '#.###.###.###.###,##', '','','','','id="vlrtotal"','','');
                            ?>
                        </td>
                    </tr>
                </table>
                <br style="clear:both;">
                <br>
                <table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
                   <thead>
                       <tr>
                           <td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
                           <td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
                           <td width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                       </tr>
                   </thead>
                       <tr bgcolor="#f7f7f7">
                           <td>
                               <? 
                                   $sql_financeiro_natureza = "SELECT SUBSTR(ndpcod,1,6) AS codigo, ndpdsc AS descricao FROM naturezadespesa WHERE edpcod != '00' AND ndpstatus = 'A' AND (sbecod = '0' OR sbecod = '00') ORDER BY ndpcod";
                                   $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'natureza\' );" ';

                                   texto_popup( 'nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 6, 13, '######', '', $complemento ); 
                               ?>
                           </td>
                           <td align="right">
                               <? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'valor\' );" '; ?>
                               <?= campo_texto( 'novo_valor', $permissao_formulario_detalhe, 'S', '', 25, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="novo_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
                           </td>
                           <td align="left">
                               <?php if ($permissao_formulario_detalhe == 'S'){ ?>
                               <a href="javascript:item_financeiro_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
                                   <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
                               </a>
                               <?php }else{ ?>
                               <a title="Incluir" style="margin: 0; padding: 0 6px;">
                                   <img src="../imagens/gif_inclui_d.gif" border="0" align="absmiddle"/>
                               </a>
                               <?php } ?>
                           </td>
                       </tr>
                   <tr>
                       <td colspan="3">
                           <div class="div_rolagem" id="div_tabela_financeiro" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
                               <table id="tabela_financeiro" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
                                   <tr valign="bottom">
                                       <td width="27%" style="height: 0; margin: 0; padding: 0;"></td>
                                       <td width="38%" style="height: 0; margin: 0; padding: 0;"></td>
                                       <td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
                                       <td width="100%" style="height: 0; margin: 0; padding: 0;"></td>
                                   </tr>
                               </table>
                           </div>
                       </td>
                   </tr>
                   <tr valign="bottom">
                       <td colspan="3" align="left" style="color: #ababab; border-top: 3px solid #dfdfdf;">
                           Valor Custeio <?= campo_texto( 'item_financeiro_valor_custeio', 'N', 'N', 'Valor Total', 19, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_custeio" ' ) ?> &nbsp;
                           Valor Capital <?= campo_texto( 'item_financeiro_valor_capital', 'N', 'N', 'Valor Total', 19, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_capital" ' ) ?> &nbsp;
                           Total <?= campo_texto( 'item_financeiro_valor_total', 'N', 'N', 'Valor Total', 19, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_total" ' ) ?>
                       </td>


                   </tr>
               </table>
            </td>
        </tr>

		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Contratações:</td>
		</tr>
		<tr id="trcontratacoes">
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Contratações:</td>
			<td>
				<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td width="25%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Forma da contratação</strong></td>
							<td width="32%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Objeto contratado</strong></td>
							<td width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Custeio</strong></td>
							<td width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Capital</strong></td>
							<td width="3%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						</tr>
					</thead>
						<tr bgcolor="#f7f7f7">
							<td width="20%">
								<div id="conitrid_on">
									<? $sql = "SELECT itrid AS codigo, itrdsc AS descricao FROM sisplan.instrumento ORDER BY 2"; ?>
									<? $db->monta_combo("conitrid",$sql,$permissao_formulario,'&nbsp','','','',200,'S','conitrid'); ?>
								</div>
								<div id="conitrid_off"></div>
							</td>
							<td>
								<div id="conobjeto_on" >
									<?= campo_texto('conobjeto', 'S', $permissao_formulario, '', 30, 200, '', '','','','','id="conobjeto"','','','');  ?>
								</div>
								<div id="conobjeto_off"></div>
							</td>
							<td align="right">
								<div id="convlrcusteio_on" >
									<?= campo_texto( "convlrcusteio", 'S', $permissao_formulario, '', 15, 15, '###.###.###.###,##', '', 'left', '', 0, "id='convlrcusteio'"); ?>
								</div>
								<div id="convlrcusteio_off"></div>
							</td>
							<td align="right">
								<div id="convlrcapital_on" >
									<?= campo_texto( "convlrcapital", 'S', $permissao_formulario, '', 15, 15, '###.###.###.###,##', '', 'left', '', 0, "id='convlrcapital'"); ?>
								</div>
								<div id="convlrcapital_off"></div>
							</td>
							<td align="left">
                                <?php if ( $permissao_formulario == 'S' ){ ?>
                                    <a onclick="item_contratacoes_adicionar_do_formulario()" title="Incluir" style="margin: 0; padding: 0 6px; cursor:pointer;">
                                        <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
                                    </a>
                                <?php }else{ ?>
                                    <a title="Incluir" style="margin: 0; padding: 0 6px;">
                                        <img src="../imagens/gif_inclui_d.gif" border="0" align="absmiddle"/>
                                    </a>
                                <?php } ?>
							</td>
						</tr>
					<tr>
						<td colspan="5">
							<div class="div_rolagem" id="div_tabela_contratacoes" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
								<table id="tabela_contratacoes" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
									<tr valign="bottom">
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="37%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="20%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Cronogramas:</td>
		</tr>
        <?php
        	$style = '';
        ?>
        <tr id="trCronograma" style="<?= $style; ?>">
            <td align='right' class="SubTituloDireita">
               &nbsp;
            </td>
            <td colspan="5">
                    <table cellspacing="0" cellpadding="0" >
                        <tr>
                            <td>
                                <?php
                                	$style = '';
                               	?>
                                <table id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 280px; float:left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Físico
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            Previsto
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("crfprevistomes{$numMes}", 'N', $permissao_formulario_detalhe, '', 22, 22, '#############', '','','','',"id='crfprevistomes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <?php
                                	$style = '';
                                ?>
                                <table id="tableOrcamentario" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 520px; float: left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Orçamentário
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("croprevistocusteiomes{$numMes}", 'N', $permissao_formulario_detalhe, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='croprevistocusteiomes{$numMes}'",'','',''); ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("croprevistocapitalmes{$numMes}", 'N', $permissao_formulario_detalhe, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='croprevistocapitalmes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <?php
                                	$style = '';
                                ?>
                                <table id="tableFinanceiro" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 520px; float: left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Financeiro
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("cfiprevistocusteiomes{$numMes}", 'N', $permissao_formulario_detalhe, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='cfiprevistocusteiomes{$numMes}'",'','',''); ?>
                                        </td>
                                        <td>
                                            <?= campo_texto("cfiprevistocapitalmes{$numMes}", 'N', $permissao_formulario_detalhe, '', 22, 22, '#.###.###.###.###,##', '','','','',"id='cfiprevistocapitalmes{$numMes}'",'','',''); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                    </table>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align="right" style="width:25%;">
                &nbsp;
            </td>
            <td colspan="7">
                <?php if($permissao_formulario_detalhe == 'S') :?>
                    <input type="button" name="btnGravar" value="Gravar" onclick="enviar();"/>
                <?php endif;?>
                <input type="button" name="btnVoltar" value="Cancelar" onclick="cancelar();"/>
            </td>
        </tr>
    </table>
</form>


<script type="text/javascript">

	var p_prgcod = '<?= $prgcod; ?>';
	var p_acacod = '<?= $acacod; ?>';
	var p_unicod = '<?= $unicod; ?>';
	var p_loccod = '<?= $loccod; ?>';
	var p_iteid = '<?= $iteid; ?>';
	var p_sbcid = '<?= $sbcid; ?>';

	jQuery(document).ready(function() {

		if (jQuery('#esfid').val() == '' ){
			document.getElementById('atisndetalharregionalizacao').disabled  = true;
		}else{
			action_esfera(jQuery('#esfid').val());
		}
		if (jQuery('#estuf').val() != '' ){
			listar_municipios(jQuery('#estuf').val(),jQuery('#hdn_muncod').val());
		}

        // verifica a exibição ou não da parte de emenda parlamentar
        if ( jQuery('#dplflgemendaparlamentar-t').attr('checked') ){
            mudaEmendaParlamentar('show');
        }else{
            mudaEmendaParlamentar('hide');
        }

        // verifica a exibição ou não da parte de meta ppa
        if ( jQuery('#dplflgcontribuippa-t').attr('checked') ){
            mudaMetaPpa('true');
        }else{
            mudaMetaPpa('false');
        }

        //reorganizando as cores da lista
        jQuery.each (jQuery('#tabela_metappa tr'), function(index, value) {
            var cor = index % 2 ? '#E9E9E9' : '#F5F5F5';
            value.style.backgroundColor = cor;
            value.onmouseout  = new Function( ' this.style.backgroundColor = "'+cor+'"; ' );
        });
	});

    /**
     * Contém os itens financeiros adicionados. Utilizado para
     * evitar repetições de natureza-iduso-fonte-idoc na tabela
     * de item financeiro.
     *
     * @var boolean[]
     */
	var item_financeiros_adicionados ={};
//	var item_financeiros_fonte_adicionados = {};
    
    /**
     * Sufixo do identificador de cada registro na tabela de
     * financeiro. Variável incremental utilizada para evitar
     * identificadores repetidos para cada tag 'tr' da tabela
     * de financeiro.
     *
     * @var float
     */
    var item_financeiro_id_atual = 0;
//    var item_financeiro_fonte_id_atual = 0;
    
    /**
     * Soma de todos os itens da tabela financeiro.
     *
     * @var float
     */
    var item_financeiro_valor_total   = 0;
    var item_financeiro_valor_custeio = 0;
    var item_financeiro_valor_capital = 0;
        
	function cancelar() {
        <?php if( $_REQUEST['acao'] == 'C'){ ?>
            window.location.href = '?modulo=principal/consultarDetalhamento&acao=<?= $_REQUEST['acao']; ?>';
        <?php }else{ ?>
            window.location.href = '?modulo=principal/consultarPlanejamento&acao=<?= $_REQUEST['acao']; ?>';
        <?php } ?>
	}


    function enviar( acao ){

            var d = document;
            var f = d.formulario;
            var msg = '';

            if( '<?php echo $permissao_formulario; ?>' == 'S'){

                if (f.uexid.value == ''){
                    msg += '\n\tUnidade Executora';
                }

                if (f.tppid.value == ''){
                    msg += '\n\tTipo de Projeto';
                }

                if (f.dplprioridade.value == ''){
                    msg += '\n\tPrioridade';
                }

                if (f.dpltitulo.value == ''){
                    msg += '\n\tTítulo';
                }

                if (f.dplobjetivo.value == ''){
                    msg += '\n\tObjetivo';
                }

                if (f.dpljustificativa.value == ''){
                    msg += '\n\tJustificativa';
                }

                if (f.dpldescricao.value == ''){
                    msg += '\n\tDescrição das Atividades';
                }

                selectAllOptions( document.getElementById('vinculacaoPlanejamento') );
                if( !jQuery('#vinculacaoPlanejamento option').val() != '' ){
                    msg += '\n\tVinculação ao Planejamento Estratégico';
                }
                
            }
            
            //validando os campos de acordo com o estado
            <?php
            if($dplid){
                $estadoAtual = wf_pegarEstadoAtual($docid);

                if ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDOAPROVACAOPI){
                    ?>
                        if (f.dplnumeropi.value == ''){
                            msg += '\n\tNúmero do PI';
                        } 
                    <?php
                }elseif ( $estadoAtual['esdid'] == WF_DETALHE_PLANEJAMENTO_ESTADO_AGUARDANDODESCENTRALIZACAO){
                    ?>
                        if (f.dplnotacredito.value == ''){
                            msg += '\n\tNúmero da nota de crédito';
                        } 
                    <?php
                }
            }
            ?>
        
            if (f.tpiid.value == ''){
                msg += '\n\tTipo de Instrumento';
            }
            
            if (f.segid.value == ''){
                msg += '\n\tSegmento Cultural';
            }
            
            if (f.mpnid.value == ''){
                msg += '\n\tMeta PNC';
            }
            
            if (f.dpldtinicio.value == ''){
                msg += '\n\tData de Início';
            }
            
            if (f.dpldttermino.value == ''){
                msg += '\n\tData de Término';
            }
            
            //se a flag de contribuição ppa for true valida ao menos um registro na lista,
            //se não for true valida o campo de texto de porque
            if ( jQuery('#dplflgcontribuippa-t').attr('checked') ){
                if ( jQuery('#tabela_metappa tr[id^="metappa_"]').length == 0 ){
		            msg += '\n\tÉ necessário o cadastro de ao menos uma Meta PPA.';
                }
		    }else{
		        if (f.dplmetappaporque.value == ''){
		            msg += '\n\tPorque?';
		        }
            }
            
            if ( jQuery('#dplflgemendaparlamentar-t').attr('checked') ){
		        if (f.tauid.value == ''){
		            msg += '\n\tTipo de autor';
		        }
		        if (f.ppoid.value == ''){
		            msg += '\n\tPartido';
		        }
		        if (f.aueid.value == ''){
		            msg += '\n\tAutor';
		        }
		    }
            
            if( '<?php echo $permissao_formulario; ?>' == 'S'){
                
                if (f.esfid.value == ''){
                    msg += '\n\tLocalização da ação';
                }

                if (f.prgcod.value == ''){
                    msg += '\n\tPrograma';
                }

                if (f.acacod.value == ''){
                    msg += '\n\tAção';
                }

                if (f.unicod.value == ''){
                    msg += '\n\tUnidade Orçamentária';
                }

                if (f.loccod.value == ''){
                    msg += '\n\tLocalizador';
                }

                if (f.ploid.value == ''){
                    msg += '\n\tPlano Orçamentário';
                }

                if (f.pduid.value == ''){
                    msg += '\n\tProduto';
                }

                if (f.udaid.value == ''){
                    msg += '\n\tUnidade Medida';
                }

                if (f.dplprosecundariometa.value == ''){
                    msg += '\n\tMeta';
                }

            }

            //validando as naturezas dos valores
            var dplvlrcusteio       = parseFloat(jQuery('#dplvlrcusteio').val().split('.').join('').split(',').join('.'));
            var dplvlrcapital       = parseFloat(jQuery('#dplvlrcapital').val().split('.').join('').split(',').join('.'));
            var valorTotalCusteio   = parseFloat(jQuery('#item_financeiro_valor_custeio').val().split('.').join('').split(',').join('.'));
            var valorTotalCapital   = parseFloat(jQuery('#item_financeiro_valor_capital').val().split('.').join('').split(',').join('.'));

            if (valorTotalCusteio != dplvlrcusteio){
                msg += '\n\tA soma dos valores de custeio das naturezas deve ser igual ao Valor de Custeio Previsto (anual).';
            }

            if (valorTotalCapital != dplvlrcapital){
                msg += '\n\tA soma dos valores de capital das contratações deve ser igual ao Valor de Capital Previsto (anual).';
            }
            
            // validando os cronogramas
            var valorTotalCusteio    = 0;
            var valorTotalCapital    = 0;
            var valorTotalCusteio2   = 0;
            var valorTotalCapital2   = 0;

            var valorPrevistoCusteio = parseFloat(jQuery('#dplvlrcusteio').val().split('.').join('').split(',').join('.'));
            var valorPrevistoCapital = parseFloat(jQuery('#dplvlrcapital').val().split('.').join('').split(',').join('.'));

            jQuery('[id^=croprevistocusteiomes]').each(function(i, elem){
                var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                if(valor){
                    valorTotalCusteio += valor;
                }
            });

            jQuery('[id^=cfiprevistocusteiomes]').each(function(i, elem){
                var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                if(valor){
                    valorTotalCusteio2 += valor;
                }
            });

            jQuery('[id^=croprevistocapitalmes]').each(function(i, elem){
                var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                if(valor){
                       valorTotalCapital += parseFloat(elem.value.split('.').join('').split(',').join('.'));
                }
            });

            jQuery('[id^=cfiprevistocapitalmes]').each(function(i, elem){
                var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                if(valor){
                       valorTotalCapital2 += parseFloat(elem.value.split('.').join('').split(',').join('.'));
                }
            });

            valorTotalCusteio  = valorTotalCusteio.toFixed(2);
            valorTotalCapital  = valorTotalCapital.toFixed(2);
            valorTotalCusteio2 = valorTotalCusteio2.toFixed(2);
            valorTotalCapital2 = valorTotalCapital2.toFixed(2);

            if(valorTotalCusteio  != valorPrevistoCusteio || valorTotalCapital  != valorPrevistoCapital
            || valorTotalCusteio2 != valorPrevistoCusteio || valorTotalCapital2 != valorPrevistoCapital){
                msg += '\n\tA soma dos valores lançados nos cronogramas Orçamentário e Financeiro deve ser igual ao Valor Previsto (anual).';
            }

            if( '<?php echo $permissao_formulario; ?>' == 'S'){
                var valorTotalCusteio = 0;
                var valorTotalCapital = 0;
                var dplvlrcusteio = parseFloat(jQuery('#dplvlrcusteio').val().split('.').join('').split(',').join('.'));
                var dplvlrcapital = parseFloat(jQuery('#dplvlrcapital').val().split('.').join('').split(',').join('.'));

                jQuery('[name^=valorcusteio]').each(function(i, elem){
                    var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                    if(valor){
                        valorTotalCusteio += parseFloat(elem.value.split('.').join('').split(',').join('.'));
                    }
                });

                jQuery('[name^=valorcapital]').each(function(i, elem){
                    var valor = parseFloat(elem.value.split('.').join('').split(',').join('.'));
                    if(valor){
                        valorTotalCapital += parseFloat(elem.value.split('.').join('').split(',').join('.'));
                    }
                });

                valorTotalCusteio = valorTotalCusteio.toFixed(2);
                valorTotalCapital = valorTotalCapital.toFixed(2);

                if (valorTotalCusteio != dplvlrcusteio){
                    msg += '\n\tA soma dos valores de custeio das contratações deve ser igual ao Valor Previsto (anual).';
                }

                if (valorTotalCapital != dplvlrcapital){
                    msg += '\n\tA soma dos valores de capital das contratações deve ser igual ao Valor Previsto (anual).';
                }
            }
            
        if (msg != ''){
            alert('Os campos listados são obrigatórios e devem ser preenchidos:' + msg);
            return false;
        }


        //atualizando o valor da hidden de ações do pac
        if ( jQuery('#ploflgappcs-t').attr('checked') ){
           jQuery('#hdn_ploflgappcs').val('t');
        }else{
           jQuery('#hdn_ploflgappcs').val('f');
        }
        
        //atualizando a hidden de regionalizacao
        if( document.getElementById('atisndetalharregionalizacao').checked ){
            jQuery('#hdn_atisndetalharregionalizacao').val('true');
        }
        
        //remove o disabled do combopopup
        jQuery("#vinculacaoPlanejamento").removeAttr("disabled");
 
        
        if( '<?php echo $permissao_formulario; ?>' == 'S'){
            // Verificando a regra de Prioridade, o sistema fará um ajax para verificar se já existe
            // Algum registro na mesma unidade
            var prioridade	= jQuery('#dplprioridade').val();
            var unidade		= jQuery('#uexid').val();
            var pplid		= jQuery('#pplid').val();

            jQuery.ajax({
                  url: "?modulo=principal/incluirPlanejamento&acao=A&act=validaPrioridade&pplid="+pplid+"&prioridade="+prioridade+"&unidade="+unidade,
                  type: "GET",
                  success: function( retorno ){
                    if ( retorno > 0 ){
                        var r = confirm("Existe uma solicitação com a prioridade informada, deseja substituir e atualizar a prioridade do projeto anterior?");
                        if ( r == true ){

                            jQuery.ajax({
                                  url: "?modulo=principal/incluirPlanejamento&acao=A&act=atualizaPrioridades&pplid="+pplid+"&prioridade="+prioridade+"&unidade="+unidade,
                                  type: "GET",
                                  success:function( retorno ){
                                    if( retorno == 'true' ){
                                        selectAllOptions( f.vinculacaoPlanejamento );
                                        document.formulario.submit();
                                    }
                                  }
                            });


                        }
                    }else{
                        selectAllOptions( f.vinculacaoPlanejamento );
                        document.formulario.submit();
                    }
                  }
            });
        }else{
            selectAllOptions( f.vinculacaoPlanejamento );
            document.formulario.submit();
        }

    }

    function calculaTotalPrevisto(){
    	var custeio = parseFloat(jQuery('#dplvlrcusteio').val().split('.').join('').split(',').join('.'));
    	var capital = parseFloat(jQuery('#dplvlrcapital').val().split('.').join('').split(',').join('.'));

    	if ( !custeio ){
    		custeio = 0;
    	}

    	if ( !capital ){
    		capital = 0;
    	}

		var total	= custeio + capital;
		jQuery('#vlrtotal').val( MascaraMonetario(new String(total)) );
    }

    //JAVASCRIPT DE REGIONALIZAÇÃO
    function action_esfera(esfid){

		if ( document.formulario.esfid.value == '' )
			document.getElementById('atisndetalharregionalizacao').disabled  = true;
		else
			//document.getElementById('atisndetalharregionalizacao').disabled  = false;



		if (document.formulario.esfid.value == 4)
		{
			document.getElementById('abrestuf_on').style.display = 'none';
			document.getElementById('abrestuf_off').style.display = '';
			document.getElementById('abrmuncod_on').style.display = 'none';
			document.getElementById('abrmuncod_off').style.display = '';
		}
		else
		{
			document.getElementById('abrestuf_on').style.display = '';
			document.getElementById('abrestuf_off').style.display = 'none';
			document.getElementById('abrmuncod_on').style.display = '';
			document.getElementById('abrmuncod_off').style.display = 'none';
		}

		if( document.getElementById('atisndetalharregionalizacao').checked == false ){
			document.getElementById('trabrangencia').style.display = 'none';
		}else{
			document.getElementById('trabrangencia').style.display = '';
		}

		esfid = esfid ? esfid : document.formulario.esfid.value;

		if(esfid=='')
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}
		if(esfid==1)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
		}
		else if(esfid==2)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value='<?=$estuf ?>';
			//document.getElementById('muncod').value='';

			listar_municipios( document.getElementById('estuf').value );

			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==3)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value  = '<?=$estuf ?>';
			document.getElementById('muncod').value = '';

			<?
			if (!$muncod){
			?>
			listar_municipios( document.getElementById('estuf').value );
			<?
			}else{
				if ($estuf == 'DF'):
			?>
			document.getElementById('muncod').value = '<?=$rgaid ?>';
			<?
				else:
			?>
			document.getElementById('muncod').value = '<?=$muncod ?>';
			<?
				endif;
			}
			?>
			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
		}
		else if(esfid==4)
		{
			document.formulario.paiid.disabled = false;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}

	}

	function listar_municipios( regcod, muncod ){

		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if(document.formulario.esfid.value!='')
		{
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';

			if ( muncod ){
				new Ajax.Request('../ajax/lista_mun_pi.php?estuf='+regcod+'&muncodi='+muncod, { method:'get'} );
			}else{
				new Ajax.Request('../ajax/lista_mun_pi.php?estuf='+regcod, { method:'get'} );
			}
		}
	}

	function exibeAbrangencia()
	{
		document.getElementById('abrpaiid').value = 'Brasil';

		if (document.getElementById('trabrangencia').style.display == 'none' )
		{
			document.getElementById('trabrangencia').style.display = '';
		}
		else
		{
			document.getElementById('trabrangencia').style.display = 'none';
		}
	}


	function listar_municipios_abrangencia( regcod )
	{
		var campo_select = document.getElementById( 'abrmuncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		ajaxGet('../ajax/lista_mun_abran.php?abrestuf='+regcod,document.getElementById('abrmuncod'),true);
	}

	///////////////////////////////////////////////
	//
	// ITENS CONTRATAÇÕES
	//
	//////////////////////////////////////////////
	var item_contratacoes_id_atual = 0;
	var item_contratacoes_adicionados = {};

	function item_contratacoes_adicionar_do_formulario()
	{
		// captura os campos
		var forma 		= document.getElementById( 'conitrid' );
		var forma_dsc 	= forma.options[forma.selectedIndex].text;
		var objeto		= document.getElementById( 'conobjeto' );
		var valorcusteio		= document.getElementById( 'convlrcusteio' );
		var valorcapital		= document.getElementById( 'convlrcapital' );

		// realiza validação dos dados
		if ( !validaBranco( forma, 'Forma de contratação' ) ) return;
		if ( !validaBranco( objeto,'Objeto contratado' ) ) return;
		if ( !validaBranco( valorcusteio, 'Valor Custeio' ) ) return;
		if ( !validaBranco( valorcapital, 'Valor Capital' ) ) return;

		// insere os dados na tabela
		if ( item_contratacoes_adicionar( forma.value, forma_dsc , objeto.value, valorcusteio.value, valorcapital.value ) == false ){
			return;
		}

		// limpa campos para nova inserção
		forma.value = '';
		objeto.value = '';
		valorcapital.value = '';
		valorcusteio.value = '';

	}

	function item_contratacoes_adicionar( forma, forma_dsc, objeto, valorcusteio, valorcapital )
	{
		// define dados gerais do item a ser adicionado
		item_contratacoes_id_atual++;
		var tabela = document.getElementById( 'tabela_contratacoes' );
		var id = 'item_contratacoes_' + item_contratacoes_id_atual;

		// cria nova linha para a tabela de financeiro
		var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
		    nova_linha.id = id;
		    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );

        var cor = jQuery('#tabela_contratacoes tr').length % 2 ? '#E9E9E9' : '#F5F5F5';
        nova_linha.onmouseout  = new Function( ' this.style.backgroundColor = "'+cor+'"; ' );
        nova_linha.style.backgroundColor = cor;
		// FIM cria nova linha para a tabela de financeiro

		// cria as célular da nova linha
		var celula_forma = nova_linha.insertCell( 0 );
		var celula_objeto = nova_linha.insertCell( 1 );
		    celula_objeto.style.textAlign = 'right';
		    celula_objeto.style.color = '#0066cc';
		var celula_valorcusteio = nova_linha.insertCell( 2 );
			celula_valorcusteio.style.textAlign = 'right';
			celula_valorcusteio.style.color = '#0066cc';
			var celula_valorcapital = nova_linha.insertCell( 3 );
			celula_valorcapital.style.textAlign = 'right';
			celula_valorcapital.style.color = '#0066cc';
		var celula_acao_alterar = nova_linha.insertCell( 4 );
		    celula_acao_alterar.style.textAlign = 'center';
		var celula_acao_remover = nova_linha.insertCell( 5 );
		    celula_acao_remover.style.textAlign = 'left';
		// FIM cria as célular da nova linha

		// cria campos ocultos com os dados do novo item
		var input_hidden_forma = criar_input( forma, 'forma' );
		var input_hidden_objeto = criar_input( objeto, 'objeto' );
		var input_hidden_valorcusteio = criar_input( valorcusteio, 'valorcusteio' );
		var input_hidden_valorcapital = criar_input( valorcapital, 'valorcapital' );
			input_hidden_forma.id = id + '_valor';
		// FIM cria campos ocultos com os dados do novo item
		// define dado para as células de açãoe

            <?php if ( $permissao_formulario == 'S' ){ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.href = 'javascript:item_contratacoes_alterar( \'' + id + '\', \'' + forma + '\', \'' + objeto + '\', \'' + valorcusteio + '\', \'' + valorcapital + '\' );';
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.href = 'javascript:item_contratacoes_remover( \'' + id + '\', true );';
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
           <?php }else{ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar_01.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir_01.gif" border="0" align="absmiddle"/>';
           <?php } ?>
               

		link_acao_alterar.id = id + '_ancora';
		// FIM define dado para as células de ação
		// insere os dados ( textos e inputs ) nas células
		celula_forma.appendChild( document.createTextNode( forma_dsc ) );
		celula_forma.appendChild( input_hidden_forma );
		celula_objeto.appendChild( document.createTextNode( objeto ) );
		celula_objeto.appendChild( input_hidden_objeto );

		celula_valorcusteio.appendChild( document.createTextNode( valorcusteio ) );
		celula_valorcusteio.appendChild( input_hidden_valorcusteio );

		celula_valorcapital.appendChild( document.createTextNode( valorcapital ) );
		celula_valorcapital.appendChild( input_hidden_valorcapital );

		if ( link_acao_alterar != null  )
		{
			celula_acao_alterar.appendChild( link_acao_alterar );
		}
		if ( link_acao_remover != null  )
		{
			celula_acao_remover.appendChild( link_acao_remover );
		}

		return true;
	}
    
	function item_contratacoes_alterar( id_tr, forma, objeto, valorcusteio, valorcapital )
	{

		// captura os campos do financeiro
		var conitrid		= document.getElementById( 'conitrid' );
		var conobjeto		= document.getElementById( 'conobjeto' );
		var convlrcusteio	= document.getElementById( 'convlrcusteio' );
		var convlrcapital	= document.getElementById( 'convlrcapital' );

		conitrid.value			= forma;
		conobjeto.value			= objeto;
		convlrcusteio.value		= valorcusteio;
		convlrcapital.value		= valorcapital;

		item_contratacoes_remover( id_tr, false );
	}

	function item_contratacoes_remover( id_tr, realizar_pergunta ){
		if ( realizar_pergunta ){
			if ( !confirm( 'Deseja realmente excluir a contratação?' ) ){
				return;
			}
		}
		houve_modificacao = true;
		var tr = document.getElementById( id_tr );
		var input = document.getElementById( id_tr + '_valor' );
		if ( tr && input ){
			tr.parentNode.removeChild( tr );
		}
	}

	///////////////////////////////////////////////
	//
	// ITENS META PPA
	//
	//////////////////////////////////////////////
    
	function metappa_adicionar_do_formulario()
	{
		// captura os campos
		var metappa         = document.getElementById( 'novo_mppid' );
		var metappa_dsc     = metappa.options[metappa.selectedIndex].text;
		var quantidade  	= document.getElementById( 'nova_quantidade' );

		// realiza validação dos dados
		if ( !validaBranco( metappa, 'Meta PPA' ) ) return;
		if ( !validaBranco( quantidade, 'Quantidade' ) ) return;

		// insere os dados na tabela
		if ( metappa_adicionar( metappa.value, metappa_dsc , quantidade.value ) == false ){
			return;
		}

		// limpa campos para nova inserção
		metappa.value = '';
		quantidade.value = '';

	}

	function metappa_adicionar( metappa, metappa_dsc, quantidade )
	{
		// define dados gerais do item a ser adicionado
		var tabela = document.getElementById( 'tabela_metappa' );
		var id = 'metappa_' + jQuery('#tabela_metappa tr[id^="metappa_"]').length + 1;

		// cria nova linha para a tabela de financeiro
		var nova_linha = tabela.insertRow( jQuery('#tabela_metappa tr[id^="metappa_"]').length + 1 );
		    nova_linha.id = id;
		    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );

        var cor = jQuery('#tabela_metappa tr[id^="metappa_"]').length % 2 ? '#E9E9E9' : '#F5F5F5';
        nova_linha.onmouseout  = new Function( ' this.style.backgroundColor = "'+cor+'"; ' );
        nova_linha.style.backgroundColor = cor;
		// FIM cria nova linha para a tabela de financeiro

		// cria as célular da nova linha
		var celula_metappa = nova_linha.insertCell( 0 );
        	celula_metappa.style.textAlign = 'left';
        	celula_metappa.width = '50%';
		var celula_quantidade = nova_linha.insertCell( 1 );
		    celula_quantidade.style.textAlign = 'right';
		    celula_quantidade.style.color = '#0066cc';
        	celula_quantidade.width = '30%';
		var celula_acao = nova_linha.insertCell( 2 );
		    celula_acao.style.textAlign = 'center';
        	celula_acao.width = '20%';
		// FIM cria as célular da nova linha

		// cria campos ocultos com os dados do novo item
		var input_hidden_metappa    = criar_input( metappa, 'metappa' );
		var input_hidden_quantidade = criar_input( quantidade, 'quantidade' );
			input_hidden_metappa.id = id + '_valor';
		// FIM cria campos ocultos com os dados do novo item
        //
		// define dado para as células de ação
            <?php if ( $permissao_formulario_detalhe == 'S' ){ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.href = 'javascript:metappa_alterar( \'' + id + '\', \'' + metappa + '\', \'' + metappa_dsc + '\',  \'' + quantidade + '\' );';
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.href = 'javascript:metappa_remover( \'' + id + '\', \'' + metappa + '\', \'' + metappa_dsc + '\', true );';
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = ' <img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
           <?php }else{ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar_01.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = ' <img src="../imagens/excluir_01.gif" border="0" align="absmiddle"/>';
           <?php } ?>
               

		link_acao_alterar.id = id + '_ancora';
		// FIM define dado para as células de ação
        // 
		// insere os dados ( textos e inputs ) nas células
		celula_metappa.appendChild( document.createTextNode( metappa_dsc ) );
		celula_metappa.appendChild( input_hidden_metappa );
        
		celula_quantidade.appendChild( document.createTextNode( quantidade ) );
		celula_quantidade.appendChild( input_hidden_quantidade );

		if ( link_acao_alterar != null  )
		{
			celula_acao.appendChild( link_acao_alterar );
		}
		if ( link_acao_remover != null  )
		{
			celula_acao.appendChild( link_acao_remover );
		}

        // Remove o elemento selecionado da combo
        jQuery('#novo_mppid option:selected').remove()

        return true;
	}
    
	function metappa_alterar( id_tr, metappa, metappa_dsc, quantidade ){

		// remove a linha a diciona novamente na combo
        metappa_remover( id_tr, metappa, metappa_dsc, false );
		
        // atribui os valores nos campos
		jQuery('#novo_mppid').val(metappa);
		jQuery('#nova_quantidade').val(quantidade);
	}

	function metappa_remover( id_tr, metappa, metappa_dsc, realizar_pergunta ){
		if ( realizar_pergunta ){
			if ( !confirm( 'Deseja realmente excluir a Meta PPA?' ) ){
				return;
			}
		}
		houve_modificacao = true;
		var tr = document.getElementById( id_tr );
		var input = document.getElementById( id_tr + '_valor' );
		if ( tr && input ){
			tr.parentNode.removeChild( tr );
            
            //adicionando o item novamente na combo
            jQuery('#novo_mppid').append(new Option(metappa_dsc, metappa));
            
            //reorganizando as cores da lista
            jQuery.each (jQuery('#tabela_metappa tr'), function(index, value) {
                var cor = index % 2 ? '#E9E9E9' : '#F5F5F5';
                value.style.backgroundColor = cor;
                value.onmouseout  = new Function( ' this.style.backgroundColor = "'+cor+'"; ' );
            });
        }
	}
















	///////////////////////////////////////////////
	//
	// ITENS ABRNGENCIA
	//
	//////////////////////////////////////////////
	var item_abrangencia_id_atual = 0;

	var item_abrangencia_adicionados = {};

	function item_abrangencia_adicionar_do_formulario()
	{
		// captura os campos
		var pais = document.getElementById( 'abrpaiid' );
		var uf 	 = document.getElementById( 'abrestuf' );
		var mun  = document.getElementById( 'abrmuncod' );

		// realiza validação dos dados
		if ( !validaBranco( pais, 'País' ) ) return;
		if( document.formulario.esfid.value != 4 )
		{
			if ( !validaBranco( uf, 'Unidade Federativa' ) ) return;
			if ( !validaBranco( mun, 'Município' ) ) return;
		}


		// insere os dados na tabela
		if ( item_abrangencia_adicionar( pais.value, uf.value, mun.value ) == false )
		{
			return;
		}
		// limpa campos para nova inserção
		pais.value = '';
		uf.value = '';
		mun.value = '';
		//item_financeiro_preencher_valores_default();
		houve_modificacao = true;
		document.getElementById( 'abrpaiid' ).focus();
	}

	function item_abrangencia_adicionar( pais, uf, mun )
	{

		var id_novo_item = pais + '_' + uf + '_' + mun;
		// verifica se item já existe
		if ( item_abrangencia_adicionados[id_novo_item] )
		{
			alert( 'Já existe uma abrangência com essas informações. Você pode alterar as informações selecionando o item correspondente na tabela abaixo do formulário.' );
			// posiciona a linha a ser destacada
			document.getElementById( item_abrangencia_adicionados[id_novo_item] + '_ancora' ).focus();
			var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + item_abrangencia_adicionados[id_novo_item] + '\', 30 ); ' );
			setTimeout( funcao_hilight, 300 );
			return false;

		}

		// define dados gerais do item a ser adicionado
		item_abrangencia_id_atual++;
		var tabela = document.getElementById( 'tabela_abrangencia' );
		var id = 'item_abragencia_' + item_abrangencia_id_atual;
		item_abrangencia_adicionados[id_novo_item] = id;

		// FIM define dados gerais do item a ser adicionado
		// cria nova linha para a tabela de financeiro
		var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
		    nova_linha.id = id;
		    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );

        var cor = jQuery('#tabela_abrangencia tr').length % 2 ? '#E9E9E9' : '#F5F5F5';
        nova_linha.onmouseout  = new Function( ' this.style.backgroundColor = "'+cor+'"; ' );
        nova_linha.style.backgroundColor = cor;
		// FIM cria nova linha para a tabela de financeiro    
            
            
		// FIM cria nova linha para a tabela de financeiro
		// cria as célular da nova linha
		var celula_pais = nova_linha.insertCell( 0 );
		var celula_uf = nova_linha.insertCell( 1 );
		    celula_uf.style.textAlign = 'right';
		    celula_uf.style.color = '#0066cc';
		var celula_mun = nova_linha.insertCell( 2 );
		    celula_mun.style.textAlign = 'right';
		    celula_mun.style.color = '#0066cc';
		var celula_acao_alterar = nova_linha.insertCell( 3 );
		    celula_acao_alterar.style.textAlign = 'center';
		var celula_acao_remover = nova_linha.insertCell( 4 );
		    celula_acao_remover.style.textAlign = 'left';
		// FIM cria as célular da nova linha
		// cria campos ocultos com os dados do novo item
		var input_hidden_pais = criar_input( pais, 'pais' );
		var input_hidden_uf = criar_input( uf, 'uf' );
		var input_hidden_mun = criar_input( mun, 'mun' );
			input_hidden_pais.id = id + '_valor';
		// FIM cria campos ocultos com os dados do novo item
		// define dado para as células de açãoe
                
           <?php if ( $permissao_formulario == 'S' ){ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.href = 'javascript:item_abrangencia_alterar( \'' + id + '\', \'' + pais + '\', \'' + uf + '\', \'' + mun + '\' );';
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.href = 'javascript:item_abrangencia_remover( \'' + id + '\', true );';
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
           <?php }else{ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar_01.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir_01.gif" border="0" align="absmiddle"/>';
           <?php } ?>

		link_acao_alterar.id = id + '_ancora';
		// FIM define dado para as células de ação
		// insere os dados ( textos e inputs ) nas células
		celula_pais.appendChild( document.createTextNode( pais ) );
		celula_pais.appendChild( input_hidden_pais );
		celula_uf.appendChild( document.createTextNode( uf ) );
		celula_uf.appendChild( input_hidden_uf );
		celula_mun.appendChild( document.createTextNode( mun ) );
		celula_mun.appendChild( input_hidden_mun );

		if ( link_acao_alterar != null  )
		{
			celula_acao_alterar.appendChild( link_acao_alterar );
		}
		if ( link_acao_remover != null  )
		{
			celula_acao_remover.appendChild( link_acao_remover );
		}

		listar_municipios_abrangencia();

		return true;
	}

	function criar_input( valor, nome_campo )
	{
		var input = document.createElement( 'input' );
		input.type = 'hidden';
		input.name = nome_campo + '[]';
		input.value = valor;
		return input;
	}

	function item_abrangencia_alterar( id_tr, pais, uf, mun )
	{
		houve_modificacao = true;
		// captura os campos do financeiro
		var campo_pais = document.getElementById( 'abrpaiid' );
		var campo_uf = document.getElementById( 'abrestuf' );
		var campo_mun = document.getElementById( 'abrmuncod' );
		listar_municipios_abrangencia(uf);
		campo_pais.value = pais;
		campo_uf.value = uf;
		campo_mun.value = mun;
		item_abrangencia_remover( id_tr, false );
	}

	function item_abrangencia_remover( id_tr, realizar_pergunta )
	{
		if ( realizar_pergunta )
		{
			if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
			{
				return;
			}
		}
		houve_modificacao = true;
		var tr = document.getElementById( id_tr );
		var input = document.getElementById( id_tr + '_valor' );
		if ( tr && input )
		{
			tr.parentNode.removeChild( tr );
		}
	}

	function item_abrangencia_hilight( id_tr, loop )
	{
		var tr = document.getElementById( id_tr );
		if ( !tr )
		{
			return;
		}
		if ( loop < 1 )
		{
			return;
		}
		var cor = '';
		switch ( loop-- % 9 )
		{
			case 0:	case 8:
				//cor = '#d0d0d0';
				cor = '#ffffff';
				break;
			case 1:	case 7:
				//cor = '#d8d8d8';
				cor = '#ffffdd';
				break;
			case 2:	case 6:
				//cor = '#e0e0e0';
				cor = '#ffffaa';
				break;
			case 3:	case 5:
				//cor = '#e8e8e8';
				cor = '#ffff80';
				break;
			case 4:
				//cor = '#f0f0f0';
				cor = '#ffff40';
				break;
		}
		tr.style.backgroundColor = cor;
		var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
		setTimeout( funcao_hilight, 120 );
	}
    //fim ITENS ABRANGENCIA

    function carregaPartido(){
		var tauid = jQuery('#tauid').val();
		jQuery('#aueid').val('');

		jQuery('#divPartido').html("carregando...");
		jQuery('#divPartido').load('?modulo=principal/incluirDetalhamento&acao=<?= $_REQUEST['acao']; ?>&act=carregaPartido&tauid='+tauid);
	}

	function carregaAutor(){
		var ppoid = jQuery('#ppoid').val();

		jQuery('#divAutor').html("carregando...");
		jQuery('#divAutor').load('?modulo=principal/incluirDetalhamento&acao=<?= $_REQUEST['acao']; ?>&act=carregaAutor&ppoid='+ppoid);
	}
    
	function mudaEmendaParlamentar( estado ){

		if(estado == 'show'){
			jQuery('.mudaEmendaParlamentar').show();
		}else{
			jQuery('.mudaEmendaParlamentar').hide();
		}
	}
    
	function mudaMetaPpa( estado ){

		if(estado == 'true'){
			jQuery('.MetaPpaTrue').show();
			jQuery('.MetaPpaFalse').hide();
		}else{
			jQuery('.MetaPpaTrue').hide();
			jQuery('.MetaPpaFalse').show();
		}
	}
    
    
	function listar_ptres(id)
	{
		if ( id != '' )
			ajaxGet('../ajax/lista_campos_ptres.php?ptres='+id,document.getElementById('span_ptres'),true);

		verificaPo();
	}

	function listar_acoes( prgcod )
	{
		var campo_select = document.getElementById( 'acacod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( prgcod != '' )
		{
			ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod+'&acacod='+p_acacod,document.getElementById('span_acacod'),true);
		}
	}

	function listar_unidades( acacod )
	{
		var campo_select = document.getElementById( 'unicod' );
		var campo_dep = document.getElementById( 'depid' );

		if( campo_select ){
			while( campo_select.options.length ){
				campo_select.options[0] = null;
			}
		}

		if ( campo_dep ){
			while( campo_dep.options.length ){
				campo_dep.options[0] = null;
			}
		}

		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_unidades.php?acacod='+acacod+'&unicod='+p_unicod,document.getElementById('span_unicod'),true);
			//listagem_produto( acacod );
			//listar_coordenador( acacod );
		}
	}

	function listagem_produto( acacod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'prodsc' );
		//while( campo_select.options.length )
		//{
		//	campo_select.options[0] = null;
		//}
		//campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod=',true);
			listar_departamentos( acacod );
		}
	}

	function listar_localizadores( unicod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'loccod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		verificaPo();

		if( unicod !='' && prgcod != '' && acacod != '')
		{
			ajaxGet('../ajax/lista_localizadores.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+p_loccod,document.getElementById('span_loccod'),true);
		}
	}

	function listar_produto( loccod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		if ( loccod != '' && prgcod != '' && acacod != '' && unicod != '' )
		{
			var url = '../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+loccod;
			ajaxGet(url, document.getElementById('codprg'),true);
		}
	}

	function verificaPo(){
		var prgcod = jQuery('#prgcod').val();
		var acacod = jQuery('#acacod').val();
		var unicod = jQuery('#unicod').val();
		var loccod = jQuery('#loccod').val();

		if(prgcod && acacod && unicod && loccod){
			jQuery('#tdPlanoOrcamentario').load('?modulo=principal/incluirPlanejamento&acao=A&act=filtraPlanoOrcamentario&prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+loccod);
			jQuery('#ploid').attr('disabled','');
		}else{
			jQuery('#ploid').val('');
			jQuery('#ploid').attr('disabled','disabled');
		}
	}

	<?php

	  if($dplid){
	  	    //Carregando Abrangência
            $modelo = new ProjetoRegionalizacao();
            $dados = $modelo->getListaRegionalizacaoByDplid((int) $dplid);

			if ( !empty($dados) ){
				?>
						document.getElementById('atisndetalharregionalizacao').checked = true;
						document.getElementById('trabrangencia').style.display = '';
				<?php

					foreach ($dados as $dado):
				?>
						item_abrangencia_adicionar('<?=$dado['pais']?>', '<?=$dado['uf']?>', '<?=$dado['mun']?>');
				<?php
					endforeach;
			}

			//Carregando Contratações
            $modelo = new ProjetoContratacao();
			$dados  = $modelo->getListaContratacoesByDplid( (int) $dplid);

			$dados = is_array($dados) ? $dados : array();
			if ( !empty($dados) ){
					foreach ($dados as $dado):
				?>
					    item_contratacoes_adicionar( <?= $dado['forma']; ?>, '<?= $dado['forma_dsc']; ?>', '<?= $dado['objeto']; ?>', '<?= formata_valor( $dado['valorcusteio']); ?>', '<?= formata_valor( $dado['valorcapital']); ?>' )
				<?php
					endforeach;
			}

			//Carregando naturezas
            $modelo = new NaturezaDetalhamento();
			$dados  = $modelo->getListaNaturezasByDplid( (int) $dplid);

			$dados = is_array($dados) ? $dados : array();
			if ( !empty($dados) ){
					foreach ($dados as $dado):
				?>
					    item_financeiro_adicionar( '<?php echo $dado['natureza']; ?>', '<?php echo formata_valor($dado['valor']); ?>' )
				<?php
					endforeach;
			}
	  }else{
 	  	    //Carregando Abrangência
            $modelo = new ProjetoRegionalizacao();
            $dados = $modelo->getListaRegionalizacaoByPplid((int) $pplid);

			if ( !empty($dados) ){
				?>
						document.getElementById('atisndetalharregionalizacao').checked = true;
						document.getElementById('trabrangencia').style.display = '';
				<?php

					foreach ($dados as $dado):
				?>
						item_abrangencia_adicionar('<?=$dado['pais']?>', '<?=$dado['uf']?>', '<?=$dado['mun']?>');
				<?php
					endforeach;
			}

			//Carregando Contratações
            $modelo = new ProjetoContratacao();
			$dados  = $modelo->getListaContratacoesByPplid( (int) $pplid);

			$dados = is_array($dados) ? $dados : array();
			if ( !empty($dados) ){
					foreach ($dados as $dado):
				?>
					    item_contratacoes_adicionar( <?= $dado['forma']; ?>, '<?= $dado['forma_dsc']; ?>', '<?= $dado['objeto']; ?>', '<?= formata_valor( $dado['valorcusteio']); ?>', '<?= formata_valor( $dado['valorcapital']); ?>' )
				<?php
					endforeach;
			}         
      }
	?>
              
    /**
     * Transforma uma string com vírgulas e pontos em um número real.
     *
     * @param string
     * @return float
     */
    function desformata_decimal( valor )
    {
        if ( valor == null )
        {
            return 0.0;
        }
        valor = valor.replace( /\./g, '' );
        valor = valor.replace( ',', '.' );
        if ( valor[valor.length] == '.' )
        {
            valor += '0';
        }
        if ( valor.length == 0 )
        {
            return 0.0;
        }
        return parseFloat( valor );
    }

    /**
     * Transforma uma string com vírgulas e pontos em um número com decimal.
     *
     * @param string
     * @return float
     */
    function desformata_numero( valor )
    {
        if ( valor == null )
        {
            return 0.0;
        }

        valor = valor.replace( /\./g, '' );
        valor = valor.replace( ',', '.' );
        if ( valor[valor.length] == '.' )
        {
            valor += '0';
        }
        if ( valor.length == 0 )
        {
            return 0.0;
        }

        return valor;
    }
        
   ///////////////////////////////////////////////
   //                                           //
   // ITENS FINANCEIROS                         //
   //                                           //
   ///////////////////////////////////////////////
    /**
     * Detecta qual o campo focado atualmente e altera para o
     * próxima caso a tecla apertada seja 'enter'.
     *
     * @param void
     * @return void
     */
    function item_financeiro_alterar_campo_focado( event, campo ){
        if ( event.keyCode != 13 )
        {
            return;
        }
        switch ( campo )
        {
            case 'natureza':
                var campo = document.getElementById( 'novo_valor' );
                break;
            case 'valor':
                item_financeiro_adicionar_do_formulario();
                return;
            default:
                return;
        }
        campo.focus();
        campo.select();
    }
    
    	// VALORES DOS CAMPOS ITEM FINANCEIRO VÁLIDOS
		var financeiro_naturezas_validas = new Array();
		<?
            $sql_financeiro_validacao = "SELECT prgcod||acacod||unicod||loccod||natureza AS codigo, natureza AS descricao FROM sisplan.financeiro WHERE finstatus = 'A' AND exercicio = '".$_SESSION['exercicio']."' ORDER BY natureza";
			$dados_temp = $db->carregar( $sql_financeiro_validacao );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>financeiro_naturezas_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>
    
    /**
     * Insere item financeiro na tabela de financeiro com
     * os dados atuais do formulário. As verificações de
     * preenchimento dos dados é realizada nesta função.
     *
     * @return void
     */
    function item_financeiro_adicionar_do_formulario(){

        var validarNatureza = <?= VALIDAR_NATUREZA_DESPESA == 'S' ? 'true' : 'false' ; ?>;
        // captura os campos do financeiro
        var natureza = document.getElementById( 'nova_natureza' );
        var valor = document.getElementById( 'novo_valor' );
        var prgcod = document.getElementById( 'prgcod' );
        var acacod = document.getElementById( 'acacod' );
        var unicod = document.getElementById( 'unicod' );
        var loccod = document.getElementById( 'loccod' );

        // realiza validação dos dados
        if ( !validaBranco( natureza, 'Natureza de Despesa' ) ) return;
        if ( !validaBranco( valor, 'Valor' ) ) return;
        if ( validarNatureza )
        {
            if ( !validaBranco( prgcod, 'Programa' ) ) return;
            if ( !validaBranco( acacod, 'Ação' ) ) return;
            if ( !validaBranco( unicod, 'Unidade' ) ) return;
            if ( !validaBranco( loccod, 'Localizador' ) ) return;

//            if ( !financeiro_naturezas_validas[prgcod.value+acacod.value+unicod.value+loccod.value+natureza.value] )
//            {
//                alert( 'A Natureza de Despesa informada não foi encontrada no orçamento de <?=$_SESSION['exercicio']?>. Se você deseja cadastrar um PA com essa Natureza de Despesa contate o administrador.' );
//                natureza.focus();
//                natureza.select();
//                return;
//            }
        }

        <?php
        if ($planejamento->pltid):
        ?>
        if ( verificaNaturezaByPlanejamento(natureza.value, valor.value) == false ){
            return;
        }
        <?php
        endif;
        ?>


        // insere os dados na tabela de financeiro
        if ( item_financeiro_adicionar( natureza.value, valor.value ) == false )
        {
            return;
        }
        // limpa campos para nova inserção
        natureza.value = '';
        valor.value = '';
        //item_financeiro_preencher_valores_default();
        houve_modificacao = true;
        document.getElementById( 'nova_natureza' ).focus();
    }
    
    /**
     * Adiciona um item à tabela de itens de despesa. Também é adiciona
     * um campo no formulário para as linhas.
     *
     * @param string
     * @param string
     * @param string
     * @param string
     * @param string
     * @return boolean
     */

    function item_financeiro_adicionar( natureza, valor ){
        if ( valor <= 0 )
        {
            alert( 'Valor deve ser maior que zero.' );
            document.getElementById( 'novo_valor' ).focus();
            return false;
        }
        var id_novo_item = natureza ;
        // verifica se item já existe
        if ( item_financeiros_adicionados[id_novo_item] )
        {
            alert( 'Já existe item financeiro para esta natureza. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
            // posiciona a linha a ser destacada
            document.getElementById( item_financeiros_adicionados[id_novo_item] + '_ancora' ).focus();
            var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados[id_novo_item] + '\', 30 ); ' );
            setTimeout( funcao_hilight, 300 );
            return false;

        }
        // define dados gerais do item a ser adicionado
        item_financeiro_id_atual++;
        var tabela = document.getElementById( 'tabela_financeiro' );
        var id = 'item_financeiro_' + item_financeiro_id_atual;
        item_financeiros_adicionados[id_novo_item] = id;
        // FIM define dados gerais do item a ser adicionado
        // cria nova linha para a tabela de financeiro
        var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
            nova_linha.id = id;
            nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
        // FIM cria nova linha para a tabela de financeiro
        // cria as célular da nova linha
        var celula_natureza = nova_linha.insertCell( 0 );
            celula_natureza.style.width = '40%';
        var celula_valor = nova_linha.insertCell( 1 );
            celula_valor.style.width = '40%';
            celula_valor.style.textAlign = 'right';
            celula_valor.style.color = '#0066cc';
        var celula_acao_alterar = nova_linha.insertCell( 2 );
            celula_acao_alterar.style.textAlign = 'center';
            celula_acao_alterar.style.width = '5%';
        var celula_acao_remover = nova_linha.insertCell( 3 );
            celula_acao_remover.style.width = '5%';
            celula_acao_remover.style.textAlign = 'left';
        // FIM cria as célular da nova linha
        // cria campos ocultos com os dados do novo item
        var input_hidden_natureza = item_financeiro_criar_input( natureza, 'natureza' );
        var input_hidden_valor = item_financeiro_criar_input( valor, 'valor' );
            input_hidden_valor.id = id + '_valor';
        // FIM cria campos ocultos com os dados do novo item
        // define dado para as células de açãoe


                
           <?php if ( $permissao_formulario_detalhe == 'S' ){ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.href = 'javascript:item_financeiro_alterar( \'' + id + '\', \'' + natureza + '\', \'' + valor + '\' );';
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.href = 'javascript:item_financeiro_remover( \'' + id + '\', \'' + natureza + '\', true );';
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
           <?php }else{ ?>
                var link_acao_alterar = document.createElement( 'a' );
                    link_acao_alterar.title = 'Editar';
                    link_acao_alterar.innerHTML = '<img src="../imagens/alterar_01.gif" border="0" align="absmiddle"/>';
                var link_acao_remover = document.createElement( 'a' );
                    link_acao_remover.title = 'Excluir';
                    link_acao_remover.innerHTML = '<img src="../imagens/excluir_01.gif" border="0" align="absmiddle"/>';
           <?php } ?>

        link_acao_alterar.id = id + '_ancora';
        // FIM define dado para as células de ação
        // insere os dados ( textos e inputs ) nas células
        celula_natureza.appendChild( document.createTextNode( natureza ) );
        celula_natureza.appendChild( input_hidden_natureza );
        celula_valor.appendChild( document.createTextNode( valor ) );
        celula_valor.appendChild( input_hidden_valor );
        if ( link_acao_alterar != null  )
        {
            celula_acao_alterar.appendChild( link_acao_alterar );
        }
        if ( link_acao_remover != null  )
        {
            celula_acao_remover.appendChild( link_acao_remover );
        }
        // FIM insere os dados ( textos e inputs ) nas células
        // altera soma dos valores de acordo com o novo item

        if ( natureza.substr(0,1) == 4 ){
            item_financeiro_valor_capital += desformata_decimal( valor );
        }
        else{
            item_financeiro_valor_custeio += desformata_decimal( valor );
        }
        
        item_financeiro_valor_total += desformata_decimal( valor );
        item_financeiro_atualizar_valor_total();
        
        
        // FIM altera soma dos valores de acordo com o novo item
        // atualiza as cores alternadas das linhas
        item_financeiro_organizar_cor_linha();
        return true;
    }
    
    /**
     * Cria o conteúdo de uma célula da tabela de itens de despesa
     *
     * @param string
     * @param string
     * @return object
     */
    function item_financeiro_criar_input( valor, nome_campo )
    {
        var input = document.createElement( 'input' );
        input.type = 'hidden';
        input.name = nome_campo + '[]';
        input.value = valor;
        return input;
    }
    
    /**
     * Atualiza o dado da tela de acordo com a variável
     * item_financeiro_valor_total.
     *
     * @param void
     * @return void
     */
    function item_financeiro_atualizar_valor_total()
    {
        var d = document;
        var valor_total_string   = item_financeiro_valor_total + '';
        var valor_custeio_string = item_financeiro_valor_custeio + '';
        var valor_capital_string = item_financeiro_valor_capital + '';

        document.getElementById( 'item_financeiro_valor_total' ).value = MascaraMonetario(valor_total_string);
        document.getElementById( 'item_financeiro_valor_custeio' ).value = MascaraMonetario(valor_custeio_string);
        document.getElementById( 'item_financeiro_valor_capital' ).value = MascaraMonetario(valor_capital_string);
    }
    
    /**
     * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
     *
     * @return void
     */
    function item_financeiro_organizar_cor_linha()
    {
        var tabela = document.getElementById( 'tabela_financeiro' );
        var quantidade_tr = tabela.rows.length;
        var cor = '';
        for ( var i = 0; i < quantidade_tr; i++ )
        {
            var tr = tabela.rows[i];
            cor = cor == '' ? '#e9e9e9' : '' ;
            tr.style.backgroundColor = cor;
            tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
        }
    }
    
    /**
     * Remove um item da tabela de financeiro.
     *
     * @param string
     * @param boolean
     * @return void
     */
    function item_financeiro_remover( id_tr, natureza, realizar_pergunta )
    {
        if ( realizar_pergunta )
        {
            if ( !confirm( 'Deseja realmente excluir a natureza?' ) )
            {
                return;
            }
        }
        item_financeiros_adicionados[natureza] = '';
        houve_modificacao = true;
        var tr = document.getElementById( id_tr );
        var input = document.getElementById( id_tr + '_valor' );
        if ( tr && input )
        {

            if ( natureza.substr(0,1) == 4 ){
                item_financeiro_valor_capital -= desformata_decimal( input.value );
            }
            else{
                item_financeiro_valor_custeio -= desformata_decimal( input.value );
            }
        
            item_financeiro_valor_total -= desformata_decimal( input.value );
            item_financeiro_atualizar_valor_total();
            tr.parentNode.removeChild( tr );
            item_financeiro_organizar_cor_linha();
        }
    }
    
    /**
     * Prepara formulário para edição de um item da tabela financeiro.
     *
     * @param string
     * @param string
     * @param string
     * @return void
     */
    function item_financeiro_alterar( id_tr, natureza, valor )
    {
        houve_modificacao = true;
        // captura os campos do financeiro
        var campo_natureza = document.getElementById( 'nova_natureza' );
        var campo_valor = document.getElementById( 'novo_valor' );
        campo_natureza.value = natureza;
        campo_valor.value = valor;
        item_financeiro_remover( id_tr, natureza, false );
        // retira item da lista dos já inseridos
        var id_novo_item = natureza;
        item_financeiros_adicionados[id_novo_item] = false;
    }

</script>