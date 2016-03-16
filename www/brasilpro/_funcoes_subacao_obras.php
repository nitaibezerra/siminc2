<?php
function salvarParObras()
{
	global $db;
//	ver($_POST,d);
	$obPreObra = new PreObra();
	$obPreObra->preid = $_POST['preid'];
	$obPreObra->predescricao = $_POST['predescricao'];
	$obPreObra->pretipofundacao = $_POST['pretipofundacao'];
	$obPreObra->presistema = OBRAS_SISID;
	$obPreObra->preidsistema = OBRAS_SISID;
	$obPreObra->ptoid = $_POST['ptoid'];
	$obPreObra->estuf = $_POST['estuf'];
	$obPreObra->muncod = $_POST['muncod_'];
	$obPreObra->estufpar = $_SESSION['brasilpro']['estuf'];
	$obPreObra->muncodpar = null;
	$obPreObra->prelogradouro = $_POST['endlog'];
	$obPreObra->precomplemento = $_POST['endcom'];
	$obPreObra->precep = str_replace(array("-","."),"",$_POST['endcep1']);
	$obPreObra->prenumero = $_POST['endnum'];
	$obPreObra->prebairro = $_POST['endbai'];
	$obPreObra->prelatitude = $_POST['latitude'] ? implode(".",$_POST['latitude']) : "null";
	$obPreObra->prelongitude = $_POST['longitude'] ? implode(".",$_POST['longitude']) : "null";
	$obPreObra->predtinclusao = "'now()";
	$obPreObra->preano = $_GET['ano'];
	$obPreObra->tooid = ORIGEM_OBRA_BRASILPRO;
	if($_POST['preid']){
		$obPreObra->salvar();
		$preid = $_POST['preid'];
	}else{
		$preid = $obPreObra->salvar();	
		preCriarDocumento($preid, FLUXO_OBRAS_BRASIL_PRO);
	}
	$obPreObra->salvarPreObraSubacao($preid,$_GET['ano'],$_GET['sbaid']);
	$obPreObra->commit();
	
	if($_POST['preid']){
		$preid = $_POST['preid'];
	}

	if($preid){
		$db->sucesso( "principal/obras/subacaoObras", "&sbaid=".$_GET['sbaid']."&ano=".$_GET['ano']."&preid=$preid" );
	}

}

function testaOrigem( $preid ){

	global $db;
	
	$sql = "SELECT
				'true'
			FROM
				obras.preobra
			WHERE
				preid = $preid 
				AND tooid = ".ORIGEM_OBRA_BRASILPRO;
	
	return $db->pegaUm($sql);
} 

function pegaEsfera( $preid ){

	global $db;
	
	$sql = "SELECT
				CASE WHEN muncodpar is null
					THEN 'E'
					ELSE 'M'
				END as esfera
			FROM
				obras.preobra
			WHERE
				preid = $preid ";
	
	return $db->pegaUm($sql);
} 

function listaObras()
{
	$oPre = new PreObra();
	if($_GET['sbaid']){
		$oPre->montaLista($_GET['sbaid'],$_GET['ano']);
	}
}

function preCriarDocumentoObrasPar( $preid ) {
	
	global $db;
	
	$docid = prePegarDocid( $preid );
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = FLUXO_OBRAS_BRASIL_PRO;
		
		// descrição do documento
		$docdsc = "Fluxo Obras do Brasil Pro";
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					obras.preobra
				SET 
					docid = {$docid} 
				WHERE
					preid = {$preid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function carregaAbasSubacaoObras($stPaginaAtual = null, $param = Array() ){
	
	global $db;
	
	$preid = $param['preid'];
	$sbaid = $param['sbaid'];
	$ano   = $param['ano'] ? $param['ano'] : pegaAno( $preid ); 
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);	
	
	$anoCorrente = ($ano == date('Y'));
	//$anoCorrente = $ano;
	
	$oSubacaoControle = new SubacaoControle();
	
	if($preid){
		$docid = prePegarDocid($preid);
		$esdid = prePegarEstadoAtual($docid);
		$isReformulacao = $db->pegaUm("SELECT preidpai FROM obras.preobra WHERE preid='".$preid."'");
		
		$lnkPendencias	= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Analise";

		$boTipoObra 			= $oSubacaoControle->verificaTipoObra($preid, 45);
		$tipoObra 				= $oSubacaoControle->verificaTipoObra($preid, 45);
		$pacFNDE 				= $oSubacaoControle->verificaObraFNDE($preid, 45);
		
		$lnkDocumento   	= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Documento";
		
		if($boTipoObra){
			
			$lnkFotos 		= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Fotos";
				
		}else{
			$lnkFotos 		= "javascript:alert(\'Informe o tipo de obra e o endereço na aba de dados do terreno.\')";	
		}
		
		$lnkDados 			= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid";
		$lnkQuestionario 	= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=Questionario";
		
	}else{
		
		$lnkDocumento 		= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkFotos 			= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		$lnkDados 			= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano";
		$lnkQuestionario 	= "javascript:alert(\'Salve os dados do terreno primeiro.\')";
		
	}
	
	$abas = array(
			0 => array("descricao" => "Dados do terreno", "link" => $lnkDados)
			);
	
	if($ano){
		
		if($preid){		
			array_push($abas, array("descricao" => "Relatório de vistoria", "link" => $lnkQuestionario));
			array_push($abas, array("descricao" => "Cadastro de fotos do terreno", "link" => $lnkFotos));
		}
		
		if($preid){
			
			array_push($abas, array("descricao" => "Documentos anexos", "link" => $lnkDocumento));
			
			//$arrEstado = Array(WF_EM_CADASTRAMENTO,WF_REVISAO_ANALISE);
			$arrEstado = Array(WF_EM_CADASTRAMENTO);
			$arrPerfilsAnalise = array(CTE_PERFIL_SUPER_USUARIO,
									   CTE_PERFIL_ADMINISTRADOR,
									   CTE_BRASIL_PROECERISTA_FNDE,
									   CTE_PERFIL_EQUIPE_LOCAL,
									   CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
									   CTE_PERFIL_EQUIPE_TECNICA);
			
			// se não for uma reformulação
			if(!$isReformulacao && $anoCorrente) {
				if( possuiPerfil( Array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR) ) ){
					array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias)); 
				}elseif( possuiPerfil( Array( CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO) ) ){
					if(  in_array( $esdid, Array(WF_EM_CADASTRAMENTO, WF_REVISAO_ANALISE, WF_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias)); 
					}
				}elseif( possuiPerfil(CTE_BRASIL_PROECERISTA_FNDE) ){
					if(  !in_array( $esdid, Array(WF_INDEFERIDO, WF_DEFERIDO,WF_DEFERIDO_CONDICIONADO_ENGENHARIA, WF_INDEFERIDO_PRAZO, WF_APROVADA, WF_ARQUIVADA) ) ){
						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias)); 
					}
				}
//				elseif( possuiPerfil(BRASIL_PRO_PERFIL_COORDENADOR_GERAL) ){
//					if(  !in_array( $esdid, Array(WF_INDEFERIDO, WF_DEFERIDO,WF_DEFERIDO_CONDICIONADO_ENGENHARIA, WF_INDEFERIDO_PRAZO, WF_APROVADA, WF_ARQUIVADA) ) ){
//						array_push($abas, array("descricao" => "Enviar para análise", "link" => $lnkPendencias)); 
//					}
//				}
			}
			
			$lnkAnaliseEngenheiro	= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano&preid=$preid&aba=AnaliseEngenheiro";
			
			if( !in_array($esdid,$arrEstado)  ){
				array_push($abas, array("descricao" => "Análise de Engenharia", "link" => $lnkAnaliseEngenheiro));	
			}
		}
	
		if(possuiPerfil(array(BRASIL_PRO_PERFIL_ADMINISTRADOR,BRASIL_PRO_PERFIL_ENGENHEIRO_FNDE))){
			
			$lnkListaObras	= "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano".($preid ? "&preid=$preid" : "" )."&aba=ListaObras";
			$titulo = "Obras no Estado";
			array_push($abas, array("descricao" => $titulo, "link" => $lnkListaObras));
		}
		
		$win = false;		
		if((in_array(BRASIL_PRO_PERFIL_SUPER_USUARIO, $perfil) ||
		    in_array(BRASIL_PRO_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		    in_array(BRASIL_PRO_PERFIL_COORDENADOR_GERAL, $perfil) ||
		    in_array(BRASIL_PRO_PERFIL_COORDENADOR_TECNICO, $perfil)) && ( $_GET['tipoAba'] == 'AnaliseEngenheiro' )){
		   	$win = true;		
		}
	}

	return montarAbasArray($abas, $stPaginaAtual, $win);			
}

function cabecalho(){
    global $db;
	
	if( $_SESSION['brasilpro']['itrid'] == 3 ){
		$sql = "SELECT
                    estdescricao as descricao
                FROM
                    territorios.estado
                WHERE
                    estuf = '".$_SESSION['brasilpro']['estuf']."'";
		$descricao = $db->pegaUm( $sql );
		$desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Descrição:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$descricao."</td>
				</tr>";
	} else {
		$sql = "SELECT
					estuf,
                    mundescricao as descricao
                FROM
                    territorios.municipio
                WHERE
                    muncod = '".$_SESSION['brasilpro']['muncod']."'";
		$municipio = $db->pegaLinha( $sql );
		$desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$municipio['descricao']."</td>
				</tr>";
	}
	
	echo "		
		<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
			<tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($municipio['estuf']) ? $municipio['estuf'] : $_SESSION['brasilpro']['estuf'])."</td>
				</tr>
				{$desc}
				{$tipoobra}
			</tbody>
		</table>
		";
}


function pegaQrpid( $preid, $queid ){
	
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	
    global $db;
   
    $sql = "SELECT
            	po.qrpid as qrpid,
            	po.predescricao as predescricao
            FROM
            	obras.preobra po
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
            WHERE
            	po.preid = {$preid}
            	AND q.queid = {$queid}";
    
    $dados = $db->pegaLinha( $sql );
    
    if( empty( $dados['qrpid'] ) ){
        $arParam = array ( "queid" => $queid, "titulo" => "OBRAS (".$preid." - ".$dados['predescricao'].")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE
                    obras.preobra
            	SET
                    qrpid = {$qrpid}
            	WHERE
                    preid = {$preid}";
    	$db->executar( $sql );
    	$db->commit();
    } else {
    	$qrpid = $dados['qrpid'];
    }
    return $qrpid;
}

function pegaAno( $preid ){
	
    global $db;
   
    $sql = "SELECT
            	preano
            FROM
            	obras.preobra po
            WHERE
            	po.preid = {$preid}";
    
    return $db->pegaUm($sql);;
}

function delimitador($texto, $valor = null){
	
	if(!ereg('[^0-9]',$texto)){
		return $texto;
	}
	
	if(!$valor){
		$valor = 280;
	}
	
	if(strlen($texto) > $valor){
			$texto = substr($texto,0,$valor).'...';
	}
		
	return $texto;
}

function montaComboMunicipioPorUf($post){
	header('content-type: text/html; charset=ISO-8859-1');

	global $db;
	
	if(!$post['estuf']){
		die($db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município'));
	}


//	if($_SESSION['brasilpro']['muncod']){
//		$where = " and muncod = '{$_SESSION['brasilpro']['muncod']}' ";
//	}

	$sql = "select
			 muncod as codigo, mundescricao as descricao
			from
			 territorios.municipio
			where
			 estuf = '".$post['estuf']."'
			 $where
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod_", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'muncod_' ));

}

function verificaCepMunicipio($post){
	
	global $db;
	
	$cep = str_replace(array('.', '-'), '', $post['cep']);
	echo $db->pegaUm("SELECT muncod FROM cep.v_endereco2 WHERE cep='".$cep."' ORDER BY cidade ASC");
	die;
}

function salvaAnotacao( $request ){
	
	global $db;
	
	$sql = "UPDATE cte.subacaoobra SET
				sobanotacoes = '".$request['sobanotacoes']."'
			WHERE
				preid = ".$request['preid'];
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			window.location = window.location;
		  </script>";
}

function verificaWFpreObra( $boMsg )
{
	if( $boMsg ){
		return 'O sistema encontrou pendências no preenchimento dos itens desta obra.';
	}
	return true;
}