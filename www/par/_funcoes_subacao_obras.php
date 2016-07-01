<?php
function salvarParObras()
{
	global $db;

	$obPreObra = new PreObra();
	$obPreObra->preid = $_POST['preid'];
	$obPreObra->predescricao = $_POST['predescricao'];
	$obPreObra->pretipofundacao = $_POST['pretipofundacao'];
	$obPreObra->presistema = $_SESSION['sisid'];
	$obPreObra->preidsistema = $_SESSION['sisid'];
	$obPreObra->ptoid =  $_POST['ptoid'];//$_POST['ptoid_disable'];
	$obPreObra->estuf = $_POST['estuf_disable'] ? $_POST['estuf_disable'] : $_POST['estuf'];
	$obPreObra->muncod = $_POST['muncod_'];
	$obPreObra->estufpar = $_SESSION['par']['estuf'];
	if( $_SESSION['par']['itrid'] == 2 ){
		$obPreObra->muncodpar = $_SESSION['par']['muncod'];
		$obPreObra->preesfera = 'M';
	} else {
		$obPreObra->preesfera = 'E';
	}
	$obPreObra->prelogradouro = $_POST['endlog'];
	$obPreObra->precomplemento = $_POST['endcom'];
	$obPreObra->prereferencia = $_POST['endreferencia'];
	$obPreObra->precep = str_replace(array("-","."),"",$_POST['endcep1']);
	$obPreObra->prenumero = $_POST['endnum'];
	$obPreObra->prebairro = $_POST['endbai'];
	$obPreObra->prelatitude = $_POST['latitude'] ? implode(".",$_POST['latitude']) : "null";
	$obPreObra->prelongitude = $_POST['longitude'] ? implode(".",$_POST['longitude']) : "null";
	$obPreObra->predtinclusao = "'now()";
	$obPreObra->preano = $_GET['ano'];
	if( $_POST['frmid_libera'] == 14 || $_POST['frmid_libera'] == 15 ){
		$obPreObra->tooid = ORIGEM_OBRA_EMENDAS;
	} else {
		$obPreObra->tooid = ORIGEM_OBRA_PAR;
	}
	if( $_POST['entcodent_'][0] ){
		$obPreObra->entcodent = $_POST['entcodent_'][0];
	}
	if($_POST['preid']){
		$obPreObra->salvar();
		$preid = $_POST['preid'];
	}else{
		$preid = $obPreObra->salvar();
		if( $_POST['frmid_libera'] == 15 ){
			$sql = "SELECT docid FROM par.subacao WHERE sbaid = ".$_REQUEST['sbaid'];
			$documentoSubacao = $db->pegaUm($sql);
			$estadoAtualSubacao = wf_pegarEstadoAtual( $documentoSubacao );
			if( $estadoAtualSubacao == WF_SUBACAO_DILIGENCIA_CONDICIONAL ){ // Aceitou condicional
				$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
						VALUES (".WF_FLUXO_OBRAS_PAR.", ".WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL.", 'Em cadastramento Condicional', now()) returning docid ";
			} else { // Aceitou normal
				$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
						VALUES (".WF_FLUXO_OBRAS_PAR.", ".WF_PAR_EM_CADASTRAMENTO.", 'Em Cadastramento', now()) returning docid ";
			}
			$docid = $db->pegaUm($sql);
			$sql = "UPDATE obras.preobra SET docid = {$docid} WHERE preid = {$preid}";
			$db->executar( $sql );
			$db->commit();
		} else {
			preCriarDocumento($preid, WF_FLUXO_OBRAS_PAR);
		}
	}
	$obPreObra->commit();
	$obPreObra->salvarPreObraSubacao($preid,$_GET['ano'],$_GET['sbaid']);
	
	if($_POST['preid']){
		$preid = $_POST['preid'];
	}

	if($preid){
		$db->sucesso( "principal/subacaoObras", "&sbaid=".$_GET['sbaid']."&ano=".$_GET['ano']."&preid=$preid" );
	//	header("Location: par.php?modulo=principal/subacaoObras&acao=A&sbaid=".$_GET['sbaid']."&ano=".$_GET['ano']."&preid=$preid");
	}

}

function listaObras()
{
	$oPre = new PreObra();
	
	if( $_GET['sbaid'] ){
		$sbaid = $_GET['sbaid']; 
	} else {
		$sbaid = $_SESSION['par']['obras']['sbaid']; 
	}
	
	$oPre->montaLista($_GET['sbaid'],$_GET['ano']);
}

function preCriarDocumentoObrasPar( $preid ) {
	
	global $db;
	
	$docid = prePegarDocid( $preid );
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = WF_FLUXO_OBRAS_PAR;
		
		// descrição do documento
		$docdsc = "Fluxo Obras do Par";
		
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
