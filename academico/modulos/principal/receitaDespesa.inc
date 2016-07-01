<?php
require_once APPRAIZ . "academico/classes/Processo.class.inc";
require_once APPRAIZ . 'academico/classes/CreditoOrcamentario.class.inc';
require_once APPRAIZ . 'academico/classes/DespesaOrcamentaria.class.inc';
require_once APPRAIZ . 'academico/classes/RecursoFinanceiroConcedente.class.inc';
require_once APPRAIZ . 'academico/classes/DespesaFinanceiraConcedente.class.inc';
require_once APPRAIZ . 'academico/classes/RecursoFinanceiroProprio.class.inc';
require_once APPRAIZ . 'academico/classes/DespesaFinanceiraProprio.class.inc';
require_once APPRAIZ . 'academico/classes/NaturezaDespesa.class.inc';
require_once APPRAIZ . 'academico/classes/DevolucaoDespesa.class.inc';
require_once APPRAIZ . 'academico/classes/DevolucaoRecurso.class.inc';

# verifica se tem permissão de gravar
$habilitado = (possuiPerfilCadastro()) ? 'S' : 'N';
$habilitaComboPopup = ($habilitado == 'S') ? false : true;

/*===========================================================================================
 * CRÉDITO ORÇAMENTÁRIO
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarCreditoOrc'){
	extract($_POST);
	
	$cdovalor = str_replace(".", "", $cdovalor);
	$cdovalor = str_replace(",", ".", $cdovalor);
	
	$obCreditoOrcamentario = new CreditoOrcamentario();
	$obCreditoOrcamentario->boJson = true;
	$obCreditoOrcamentario->cdoid = $cdoid;
  	$obCreditoOrcamentario->prcid = $prcidCreditoOrc; 
  	$obCreditoOrcamentario->ndpcod = $ndpcodCreditoOrc;
  	$obCreditoOrcamentario->cdovalor = $cdovalor;
  	$obCreditoOrcamentario->cdodtinclusao = date('Y-m-d'); 
  	$obCreditoOrcamentario->cdostatus = 'A'; 
	$cdoidTemp = $obCreditoOrcamentario->salvar();
	$cdoid = ($cdoid) ? $cdoid : $cdoidTemp;
	if($obCreditoOrcamentario->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obCreditoOrcamentario->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arCreditoOrcamentario = array();
		$arCreditoOrcamentario['msg'] = 'sucesso';
		$arCreditoOrcamentario['rowIndexCreditoOrc'] = $rowIndexCreditoOrc;
		$arCreditoOrcamentario['cdoid'] = $cdoid;
		$arCreditoOrcamentario['ndpcod'] = $ndpcod;
		$arCreditoOrcamentario['cdovalor'] = number_format($obCreditoOrcamentario->cdovalor,2,',','.');
		echo simec_json_encode($arCreditoOrcamentario);
	} else {
		$obCreditoOrcamentario->rollback();
		$arCreditoOrcamentario['msg'] = 'erro';
		echo simec_json_encode($arCreditoOrcamentario);
	}
	unset($obCreditoOrcamentario);
	die;
}

if($_POST['requisicao'] == 'carregarCreditoOrc'){
	extract($_POST);
	$obCreditoOrcamentario = new CreditoOrcamentario($cdoid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obCreditoOrcamentario->ndpid){
		$obCreditoOrcamentario->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obCreditoOrcamentario->ndpid);		
	}
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obCreditoOrcamentario->ndpid} ";
	$ndpidCreditoOrc_dsc = $db->pegaUm($sql);*/
	
	$arCreditoOrcamentario = array();
	$arCreditoOrcamentario['ndpcodCreditoOrc']     = $obCreditoOrcamentario->ndpcod;
//	$arCreditoOrcamentario['ndpcodCreditoOrc_dsc'] = utf8_encode($ndpidCreditoOrc_dsc);
	$arCreditoOrcamentario['cdovalor']  		  = number_format($obCreditoOrcamentario->cdovalor,2,',','.');
	echo simec_json_encode($arCreditoOrcamentario);
		
	unset($obCreditoOrcamentario);
	die;
}

if($_POST['requisicao'] == 'excluirCreditoOrc'){
	extract($_POST);
	
	if($cdoid){
		$obCreditoOrcamentario = new CreditoOrcamentario($cdoid);
		$obCreditoOrcamentario->excluir($cdoid);
		if($obCreditoOrcamentario->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obCreditoOrcamentario);		
	}
	die;
}

/*===========================================================================================
 * CRÉDITO ORÇAMENTÁRIO
 *===========================================================================================
 */


/*===========================================================================================
 * DESPESA ORÇAMENTARIA
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarDespesaOrc'){
	extract($_POST);
	
	$dpovalor = str_replace(".", "", $dpovalor);
	$dpovalor = str_replace(",", ".", $dpovalor);
	
	$obDespesaOrcamentaria = new DespesaOrcamentaria();
	$obDespesaOrcamentaria->boJson = true;
	$obDespesaOrcamentaria->dpoid = $dpoid;
  	$obDespesaOrcamentaria->prcid = $prcidDespesaOrc; 
  	$obDespesaOrcamentaria->ndpcod = $ndpcodDespesaOrc;
  	$obDespesaOrcamentaria->dpovalor = $dpovalor;
  	$obDespesaOrcamentaria->dpodtinclusao = date('Y-m-d'); 
  	$obDespesaOrcamentaria->dpostatus = 'A';
	$dpoidTemp = $obDespesaOrcamentaria->salvar();
	$dpoid = ($dpoid) ? $dpoid : $dpoidTemp;
	if($obDespesaOrcamentaria->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obDespesaOrcamentaria->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arDespesaOrcamentaria = array();
		$arDespesaOrcamentaria['msg'] = 'sucesso';
		$arDespesaOrcamentaria['rowIndexDespesaOrc'] = $rowIndexDespesaOrc;
		$arDespesaOrcamentaria['dpoid'] = $dpoid;
		$arDespesaOrcamentaria['ndpcod'] = $ndpcod;
		$arDespesaOrcamentaria['dpovalor'] = number_format($obDespesaOrcamentaria->dpovalor,2,',','.');
		echo simec_json_encode($arDespesaOrcamentaria);
	} else {
		$obDespesaOrcamentaria->rollback();
		$arDespesaOrcamentaria['msg'] = 'erro';
		echo simec_json_encode($arDespesaOrcamentaria);
	}
	
	unset($obDespesaOrcamentaria);
	die;
}

if($_POST['requisicao'] == 'carregarDespesaOrc'){
	extract($_POST);
	$obDespesaOrcamentaria = new DespesaOrcamentaria($dpoid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obDespesaOrcamentaria->ndpid){
		$obDespesaOrcamentaria->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obDespesaOrcamentaria->ndpid);		
	}
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obDespesaOrcamentaria->ndpid} ";
	$ndpidDespesaOrc_dsc = $db->pegaUm($sql);*/
	
	$arDespesaOrcamentaria = array();
	$arDespesaOrcamentaria['ndpcodDespesaOrc']     = $obDespesaOrcamentaria->ndpcod;
//	$arDespesaOrcamentaria['ndpcodDespesaOrc_dsc'] = utf8_encode($ndpidDespesaOrc_dsc);
	$arDespesaOrcamentaria['dpovalor']  		  = number_format($obDespesaOrcamentaria->dpovalor,2,',','.');
	echo simec_json_encode($arDespesaOrcamentaria);
		
	unset($obDespesaOrcamentaria);
	die;
}

if($_POST['requisicao'] == 'excluirDespesaOrc'){
	extract($_POST);
	
	if($dpoid){
		$obDespesaOrcamentaria = new DespesaOrcamentaria($dpoid);
		$obDespesaOrcamentaria->excluir($dpoid);
		if($obDespesaOrcamentaria->commit()){
			echo 'sucesso';			
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obDespesaOrcamentaria);
	}
	die;
}

/*===========================================================================================
 * DESPESA ORÇAMENTARIA
 *===========================================================================================
 */


/*===========================================================================================
 * RECURSO FINANCEIRO CONCEDENTE
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarRecursoFinConc'){
	extract($_POST);
	
	$rfcvalor = str_replace(".", "", $rfcvalor);
	$rfcvalor = str_replace(",", ".", $rfcvalor);
	
	$obRecursoFinanceiroConcedente = new RecursoFinanceiroConcedente();
	$obRecursoFinanceiroConcedente->boJson = true;
	$obRecursoFinanceiroConcedente->rfcid = $rfcid;
  	$obRecursoFinanceiroConcedente->prcid = $prcidRecursoFinConc; 
  	$obRecursoFinanceiroConcedente->ndpcod = $ndpcodRecursoFinConc;
  	$obRecursoFinanceiroConcedente->rfcvalor = $rfcvalor;
  	$obRecursoFinanceiroConcedente->rfcdtinclusao = date('Y-m-d'); 
  	$obRecursoFinanceiroConcedente->rfcstatus = 'A'; 
	$rfcid = $obRecursoFinanceiroConcedente->salvar();
	$rfcid = ($rfcid) ? $rfcid : $rfcidTemp;
	if($obRecursoFinanceiroConcedente->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obRecursoFinanceiroConcedente->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arRecursoFinanceiroConcedente = array();
		$arRecursoFinanceiroConcedente['msg'] = 'sucesso';
		$arRecursoFinanceiroConcedente['rowIndexRecursoFinConc'] = $rowIndexRecursoFinConc;
		$arRecursoFinanceiroConcedente['rfcid'] = $rfcid;
		$arRecursoFinanceiroConcedente['ndpcod'] = $ndpcod;
		$arRecursoFinanceiroConcedente['rfcvalor'] = number_format($obRecursoFinanceiroConcedente->rfcvalor,2,',','.');
		echo simec_json_encode($arRecursoFinanceiroConcedente);
	} else {
		$obRecursoFinanceiroConcedente->rollback();
		$arRecursoFinanceiroConcedente['msg'] = 'erro';
		echo simec_json_encode($arRecursoFinanceiroConcedente);
	}
	unset($obRecursoFinanceiroConcedente);
	die;
}

if($_POST['requisicao'] == 'carregarRecursoFinConc'){
	extract($_POST);
	$obRecursoFinanceiroConcedente = new RecursoFinanceiroConcedente($rfcid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obRecursoFinanceiroConcedente->ndpid){
		$obRecursoFinanceiroConcedente->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obRecursoFinanceiroConcedente->ndpid);		
	}
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obRecursoFinanceiroConcedente->ndpid} ";
	$ndpidRecursoFinConc_dsc = $db->pegaUm($sql);*/
	
	$arRecursoFinanceiroConcedente = array();
	$arRecursoFinanceiroConcedente['ndpcodRecursoFinConc']     = $obRecursoFinanceiroConcedente->ndpcod;
//	$arRecursoFinanceiroConcedente['ndpcodRecursoFinConc_dsc'] = utf8_encode($ndpidRecursoFinConc_dsc);
	$arRecursoFinanceiroConcedente['rfcvalor']  			  = number_format($obRecursoFinanceiroConcedente->rfcvalor,2,',','.');
	echo simec_json_encode($arRecursoFinanceiroConcedente);
		
	unset($obRecursoFinanceiroConcedente);
	die;
}

if($_POST['requisicao'] == 'excluirRecursoFinConc'){
	extract($_POST);
	
	if($rfcid){
		$obRecursoFinanceiroConcedente = new RecursoFinanceiroConcedente($rfcid);
		$obRecursoFinanceiroConcedente->excluir($rfcid);
		if($obRecursoFinanceiroConcedente->commit()){
			echo 'sucesso';			
		} else {
			$obRecursoFinanceiroConcedente->rollback();
		}
		unset($obRecursoFinanceiroConcedente);
	}
	die;
}

/*===========================================================================================
 * RECURSO FINANCEIRO CONCEDENTE
 *===========================================================================================
 */


/*===========================================================================================
 * DESPESA FINANCEIRA CONCEDENTE
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarDespFinanConc'){
	extract($_POST);
	
	$dfcvalor = str_replace(".", "", $dfcvalor);
	$dfcvalor = str_replace(",", ".", $dfcvalor);
	
	$obDespesaFinanceiraConcedente = new DespesaFinanceiraConcedente();
	$obDespesaFinanceiraConcedente->boJson = true;
	$obDespesaFinanceiraConcedente->dfcid = $dfcid;
  	$obDespesaFinanceiraConcedente->prcid = $prcidDespFinanConc; 
  	$obDespesaFinanceiraConcedente->ndpcod = $ndpcodDespFinanConc;
  	$obDespesaFinanceiraConcedente->dfcvalor = $dfcvalor;
  	$obDespesaFinanceiraConcedente->dfcdtinclusao = date('Y-m-d'); 
  	$obDespesaFinanceiraConcedente->dfcstatus = 'A';
	$dfcidTemp = $obDespesaFinanceiraConcedente->salvar();
	$dfcid = ($dfcid) ? $dfcid : $dfcidTemp;
	if($obDespesaFinanceiraConcedente->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obDespesaFinanceiraConcedente->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arDespesaFinanceiraConcedente = array();
		$arDespesaFinanceiraConcedente['msg'] = 'sucesso';
		$arDespesaFinanceiraConcedente['rowIndexDespFinanConc'] = $rowIndexDespFinanConc;
		$arDespesaFinanceiraConcedente['dfcid'] = $dfcid;
		$arDespesaFinanceiraConcedente['ndpcod'] = $ndpcod;
		$arDespesaFinanceiraConcedente['dfcvalor'] = number_format($obDespesaFinanceiraConcedente->dfcvalor,2,',','.');
		echo simec_json_encode($arDespesaFinanceiraConcedente);
	} else {
		$obDespesaFinanceiraConcedente->rollback();
		$arDespesaFinanceiraConcedente['msg'] = 'erro';
		echo simec_json_encode($arDespesaFinanceiraConcedente);
	}
	unset($obDespesaFinanceiraConcedente);
	die;
}

if($_POST['requisicao'] == 'carregarDespFinanConc'){
	extract($_POST);
	$obDespesaFinanceiraConcedente = new DespesaFinanceiraConcedente($dfcid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obDespesaFinanceiraConcedente->ndpid){
		$obDespesaFinanceiraConcedente->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obDespesaFinanceiraConcedente->ndpid);		
	}	
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obDespesaFinanceiraConcedente->ndpid} ";
	$ndpidDespFinanConc_dsc = $db->pegaUm($sql);*/
	
	$arDespesaFinanceiraConcedente = array();
	$arDespesaFinanceiraConcedente['ndpcodDespFinanConc']     = $obDespesaFinanceiraConcedente->ndpcod;
//	$arDespesaFinanceiraConcedente['ndpcodDespFinanConc_dsc'] = utf8_encode($ndpidDespFinanConc_dsc);
	$arDespesaFinanceiraConcedente['dfcvalor']  			 = number_format($obDespesaFinanceiraConcedente->dfcvalor,2,',','.');
	echo simec_json_encode($arDespesaFinanceiraConcedente);
		
	unset($obDespesaFinanceiraConcedente);
	die;
}

if($_POST['requisicao'] == 'excluirDespFinanConc'){
	extract($_POST);
	
	if($dfcid){
		$obDespesaFinanceiraConcedente = new DespesaFinanceiraConcedente($dfcid);
		$obDespesaFinanceiraConcedente->excluir($dfcid);
		if($obDespesaFinanceiraConcedente->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obDespesaFinanceiraConcedente);		
	}
	die;
}

/*===========================================================================================
 * DESPESA FINANCEIRA CONCEDENTE
 *===========================================================================================
 */


/*===========================================================================================
 * RECURSO FINANCEIRO PRÓPRIO
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarRecursoFinanPro'){
	extract($_POST);
	
	$rfpvalor = str_replace(".", "", $rfpvalor);
	$rfpvalor = str_replace(",", ".", $rfpvalor);
	
	$obRecursoFinanceiroProprio = new RecursoFinanceiroProprio();
	$obRecursoFinanceiroProprio->boJson = true;
	$obRecursoFinanceiroProprio->rfpid = $rfpid;
  	$obRecursoFinanceiroProprio->prcid = $prcidRecursoFinanPro; 
  	$obRecursoFinanceiroProprio->ndpcod = $ndpcodRecursoFinanPro;
  	$obRecursoFinanceiroProprio->rfpvalor = $rfpvalor;
  	$obRecursoFinanceiroProprio->rfpdtinclusao = date('Y-m-d'); 
  	$obRecursoFinanceiroProprio->rfpstatus = 'A'; 
	$rfpidTemp = $obRecursoFinanceiroProprio->salvar();
	$rfpid = ($rfpid) ? $rfpid : $rfpidTemp;
	if($obRecursoFinanceiroProprio->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obRecursoFinanceiroProprio->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arRecursoFinanceiroProprio = array();
		$arRecursoFinanceiroProprio['msg'] = 'sucesso';
		$arRecursoFinanceiroProprio['rowIndexRecursoFinanPro'] = $rowIndexRecursoFinanPro;
		$arRecursoFinanceiroProprio['rfpid'] = $rfpid;
		$arRecursoFinanceiroProprio['ndpcod'] = $ndpcod;
		$arRecursoFinanceiroProprio['rfpvalor'] = number_format($obRecursoFinanceiroProprio->rfpvalor,2,',','.');
		echo simec_json_encode($arRecursoFinanceiroProprio);
	} else {
		$obRecursoFinanceiroProprio->rollback();
		$arRecursoFinanceiroProprio['msg'] = 'erro';
		echo simec_json_encode($arRecursoFinanceiroProprio);
	}
	unset($obRecursoFinanceiroProprio);
	die;
}

if($_POST['requisicao'] == 'carregarRecursoFinanPro'){
	extract($_POST);
	$obRecursoFinanceiroProprio = new RecursoFinanceiroProprio($rfpid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obRecursoFinanceiroProprio->ndpid){
		$obRecursoFinanceiroProprio->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obRecursoFinanceiroProprio->ndpid);		
	}
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obRecursoFinanceiroProprio->ndpid} ";
	$ndpidRecursoFinanPro_dsc = $db->pegaUm($sql);*/
	
	$arRecursoFinanceiroProprio = array();
	$arRecursoFinanceiroProprio['ndpcodRecursoFinanPro']     = $obRecursoFinanceiroProprio->ndpcod;
//	$arRecursoFinanceiroProprio['ndpcodRecursoFinanPro_dsc'] = utf8_encode($ndpidRecursoFinanPro_dsc);
	$arRecursoFinanceiroProprio['rfpvalor']  				= number_format($obRecursoFinanceiroProprio->rfpvalor,2,',','.');
	echo simec_json_encode($arRecursoFinanceiroProprio);
		
	unset($obRecursoFinanceiroProprio);
	die;
}

if($_POST['requisicao'] == 'excluirRecursoFinanPro'){
	extract($_POST);
	
	if($rfpid){
		$obRecursoFinanceiroProprio = new RecursoFinanceiroProprio($rfpid);
		$obRecursoFinanceiroProprio->excluir($rfpid);
		if($obRecursoFinanceiroProprio->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obRecursoFinanceiroProprio);		
	}
	die;
}

/*===========================================================================================
 * RECURSO FINANCEIRO PRÓPRIO
 *===========================================================================================
 */


/*===========================================================================================
 * DESPESA FINANCEIRA PRÓPRIO
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarDespesaFinanPro'){
	extract($_POST);
	
	$dfpvalor = str_replace(".", "", $dfpvalor);
	$dfpvalor = str_replace(",", ".", $dfpvalor);
	
	$obDespesaFinanceiraProprio = new DespesaFinanceiraProprio();
	$obDespesaFinanceiraProprio->boJson = true;
	$obDespesaFinanceiraProprio->dfpid = $dfpid;
  	$obDespesaFinanceiraProprio->prcid = $prcidDespesaFinanPro; 
  	$obDespesaFinanceiraProprio->ndpcod = $ndpcodDespesaFinanPro;
  	$obDespesaFinanceiraProprio->dfpvalor = $dfpvalor;
  	$obDespesaFinanceiraProprio->dfpdtinclusao = date('Y-m-d'); 
  	$obDespesaFinanceiraProprio->dfpstatus = 'A';
	$dfpidTemp = $obDespesaFinanceiraProprio->salvar();
	$dfpid = ($dfpid) ? $dfpid : $dfpidTemp;
	if($obDespesaFinanceiraProprio->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obDespesaFinanceiraProprio->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arDespesaFinanceiraProprio = array();
		$arDespesaFinanceiraProprio['msg'] = 'sucesso';
		$arDespesaFinanceiraProprio['rowIndexDespesaFinanPro'] = $rowIndexDespesaFinanPro;
		$arDespesaFinanceiraProprio['dfpid'] = $dfpid;
		$arDespesaFinanceiraProprio['ndpcod'] = $ndpcod;
		$arDespesaFinanceiraProprio['dfpvalor'] = number_format($obDespesaFinanceiraProprio->dfpvalor,2,',','.');
		echo simec_json_encode($arDespesaFinanceiraProprio);
	} else {
		$obDespesaFinanceiraProprio->rollback();
		$arDespesaFinanceiraProprio['msg'] = 'erro';
		echo simec_json_encode($arDespesaFinanceiraProprio);
	}
	unset($obDespesaFinanceiraProprio);
	die;
}

if($_POST['requisicao'] == 'carregarDespesaFinanPro'){
	extract($_POST);
	$obDespesaFinanceiraProprio = new DespesaFinanceiraProprio($dfpid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obDespesaFinanceiraProprio->ndpid){
		$obDespesaFinanceiraProprio->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obDespesaFinanceiraProprio->ndpid);		
	}
	
	//$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obDespesaFinanceiraProprio->ndpid} ";
	//$ndpidDespesaFinanPro_dsc = $db->pegaUm($sql);
	
	$arDespesaFinanceiraProprio = array();
	$arDespesaFinanceiraProprio['ndpcodDespesaFinanPro']     = $obDespesaFinanceiraProprio->ndpcod;
	//$arDespesaFinanceiraProprio['ndpcodDespesaFinanPro_dsc'] = utf8_encode($ndpidDespesaFinanPro_dsc);
	$arDespesaFinanceiraProprio['dfpvalor']  		  = number_format($obDespesaFinanceiraProprio->dfpvalor,2,',','.');
	echo simec_json_encode($arDespesaFinanceiraProprio);
		
	unset($obDespesaFinanceiraProprio);
	die;
}

if($_POST['requisicao'] == 'excluirDespesaFinanPro'){
	extract($_POST);
	
	if($dfpid){
		$obDespesaFinanceiraProprio = new DespesaFinanceiraProprio($dfpid);
		$obDespesaFinanceiraProprio->excluir($dfpid);
		if($obDespesaFinanceiraProprio->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obDespesaFinanceiraProprio);		
	}
	die;
}

/*===========================================================================================
 * DESPESA FINANCEIRA PRÓPRIO
 *===========================================================================================
 */

/*===========================================================================================
 * DEVOLUCAO RECURSO
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarDevolucaoRecur'){
	extract($_POST);
	
	$dervalor = str_replace(".", "", $dervalor);
	$dervalor = str_replace(",", ".", $dervalor);
	
	$obDevolucaoRecurso = new DevolucaoRecurso();
	$obDevolucaoRecurso->boJson = true;
	$obDevolucaoRecurso->derid = $derid;
  	$obDevolucaoRecurso->prcid = $prcidDevolucaoRecur; 
  	$obDevolucaoRecurso->ndpcod = $ndpcodDevolucaoRecur;
  	$obDevolucaoRecurso->dervalor = $dervalor;
  	$obDevolucaoRecurso->derdtinclusao = date('Y-m-d'); 
  	$obDevolucaoRecurso->derstatus = 'A'; 
	$deridTemp = $obDevolucaoRecurso->salvar();
	$derid = ($derid) ? $derid : $deridTemp;
	if($obDevolucaoRecurso->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obDevolucaoRecurso->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arDevolucaoRecurso = array();
		$arDevolucaoRecurso['msg'] = 'sucesso';
		$arDevolucaoRecurso['rowIndexDevolucaoRecur'] = $rowIndexDevolucaoRecur;
		$arDevolucaoRecurso['derid'] = $derid;
		$arDevolucaoRecurso['ndpcod'] = $ndpcod;
		$arDevolucaoRecurso['dervalor'] = number_format($obDevolucaoRecurso->dervalor,2,',','.');
		echo simec_json_encode($arDevolucaoRecurso);
	} else {
		$obDevolucaoRecurso->rollback();
		$arDevolucaoRecurso['msg'] = 'erro';
		echo simec_json_encode($arDevolucaoRecurso);
	}
	unset($obDevolucaoRecurso);
	die;
}

if($_POST['requisicao'] == 'carregarDevolucaoRecur'){
	extract($_POST);
	$obDevolucaoRecurso = new DevolucaoRecurso($derid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obDevolucaoRecurso->ndpid){
		$obDevolucaoRecurso->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obDevolucaoRecurso->ndpid);		
	}
	
	/*$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obDevolucaoRecurso->ndpid} ";
	$ndpidDevolucaoRecur_dsc = $db->pegaUm($sql);*/
	
	$arDevolucaoRecurso = array();
	$arDevolucaoRecurso['ndpcodDevolucaoRecur']     = $obDevolucaoRecurso->ndpcod;
//	$arDevolucaoRecurso['ndpcodDevolucaoRecur_dsc'] = utf8_encode($ndpidDevolucaoRecur_dsc);
	$arDevolucaoRecurso['dervalor']  				= number_format($obDevolucaoRecurso->dervalor,2,',','.');
	echo simec_json_encode($arDevolucaoRecurso);
		
	unset($obDevolucaoRecurso);
	die;
}

if($_POST['requisicao'] == 'excluirDevolucaoRecur'){
	extract($_POST);
	
	if($derid){
		$obDevolucaoRecurso = new DevolucaoRecurso($derid);
		$obDevolucaoRecurso->excluir($derid);
		if($obDevolucaoRecurso->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obDevolucaoRecurso);		
	}
	die;
}

/*===========================================================================================
 * DEVOLUCAO RECURSO
 *===========================================================================================
 */

/*===========================================================================================
 * DEVOLUCAO DESPESA
 *===========================================================================================
 */
if($_POST['requisicao'] == 'salvarDevolucaoDesp'){
	extract($_POST);
	
	$dedvalor = str_replace(".", "", $dedvalor);
	$dedvalor = str_replace(",", ".", $dedvalor);
	
	$obDevolucaoDespesa = new DevolucaoDespesa();
	$obDevolucaoDespesa->boJson = true;
	$obDevolucaoDespesa->dedid = $dedid;
  	$obDevolucaoDespesa->prcid = $prcidDevolucaoDesp; 
  	$obDevolucaoDespesa->ndpcod = $ndpcodDevolucaoDesp;
  	$obDevolucaoDespesa->dedvalor = $dedvalor;
  	$obDevolucaoDespesa->deddtinclusao = date('Y-m-d'); 
  	$obDevolucaoDespesa->dedstatus = 'A';
	$dedidTemp = $obDevolucaoDespesa->salvar();
	$dedid = ($dedid) ? $dedid : $dedidTemp;
	if($obDevolucaoDespesa->commit()){
		$sql = "select ndpcod from public.naturezadespesa where ndpid = {$obDevolucaoDespesa->ndpid}";
		$ndpcod = $db->pegaUm($sql);
		$arDevolucaoDespesa = array();
		$arDevolucaoDespesa['msg'] = 'sucesso';
		$arDevolucaoDespesa['rowIndexDevolucaoDesp'] = $rowIndexDevolucaoDesp;
		$arDevolucaoDespesa['dedid'] = $dedid;
		$arDevolucaoDespesa['ndpcod'] = $ndpcod;
		$arDevolucaoDespesa['dedvalor'] = number_format($obDevolucaoDespesa->dedvalor,2,',','.');
		echo simec_json_encode($arDevolucaoDespesa);
	} else {
		$obDevolucaoDespesa->rollback();
		$arDevolucaoDespesa['msg'] = 'erro';
		echo simec_json_encode($arDevolucaoDespesa);
	}
	unset($obDevolucaoDespesa);
	die;
}

if($_POST['requisicao'] == 'carregarDevolucaoDesp'){
	extract($_POST);
	$obDevolucaoDespesa = new DevolucaoDespesa($dedid);
	
	$obNaturezaDespesa = new NaturezaDespesa();
	if($obDevolucaoDespesa->ndpid){
		$obDevolucaoDespesa->ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obDevolucaoDespesa->ndpid);		
	}
	
	//$sql= "select  ndpcod||' - '||ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A' and ndpid = {$obDevolucaoDespesa->ndpid} ";
	//$ndpidDevolucaoDesp_dsc = $db->pegaUm($sql);
	
	$arDevolucaoDespesa = array();
	$arDevolucaoDespesa['ndpcodDevolucaoDesp']     = $obDevolucaoDespesa->ndpcod;
	//$arDevolucaoDespesa['ndpcodDevolucaoDesp_dsc'] = utf8_encode($ndpidDevolucaoDesp_dsc);
	$arDevolucaoDespesa['dedvalor']  		  = number_format($obDevolucaoDespesa->dedvalor,2,',','.');
	echo simec_json_encode($arDevolucaoDespesa);
		
	unset($obDevolucaoDespesa);
	die;
}

if($_POST['requisicao'] == 'excluirDevolucaoDesp'){
	extract($_POST);
	
	if($dedid){
		$obDevolucaoDespesa = new DevolucaoDespesa($dedid);
		$obDevolucaoDespesa->excluir($dedid);
		if($obDevolucaoDespesa->commit()){
			echo 'sucesso';		
		} else {
			$obDespesaOrcamentaria->rollback();
		}
		unset($obDevolucaoDespesa);		
	}
	die;
}

/*===========================================================================================
 * DEVOLUCAO DESPESA
 *===========================================================================================
 */


require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

echo montarAbasArray( criaAbaPortaria(), "academico.php?modulo=principal/receitaDespesa&acao=A" );

$titulo_modulo = "RELATÓRIO DE EXECUÇÃO DA RECEITA E DESPESA";
monta_titulo( $titulo_modulo, 'Receita e Despesa' );

$obProcesso = new Processo($_SESSION['academico']['prcid']);

include_once APPRAIZ . "includes/workflow.php";
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-fixedheadertable/fixedHeaderTable.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-fixedheadertable/jquery.fixedheadertable.1.0.js"></script>
<script type="text/javascript" src="./geral/js/receitaDespesa.js"></script>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
	<tbody>
		<tr>
			<td width="50%" valign="top"><strong>Executor N°: <?php echo $db->pegaUm("select unicod || ' - ' || unidsc as descricao from public.unidade where unistatus = 'A' and unicod = '{$obProcesso->unicodexecutor}'"); ?></strong></td>
			<td>
				<table>
					<tr>
						<td width="200px" class="SubTituloDireita" valign="bottom">Portaria N°:</td>
						<td width="500px"><?php echo "<strong>".$obProcesso->prtid."</strong>"; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" valign="bottom">Periodo de Execução:</td>
						<td>
							<?php 
								echo "<strong>".$obProcesso->prndtinicio.' - '.$obProcesso->prndtfim."</strong>"; 
							?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="text-align:center"><strong>CRÉDITO ORÇAMENTÁRIO</strong></td>
			<td class="SubTituloDireita" style="text-align:center" colspan="2"><strong>DESPESA ORÇAMENTÁRIA</strong></td>
		</tr>
		<tr>
			<!-- CRÉDITO ORÇAMENTÁRIO -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollCreditoOrc" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formCreditoOrc" name="formCreditoOrc" method="post" action="">
											<input type="hidden" name="rowIndexCreditoOrc" id="rowIndexCreditoOrc" value="">
											<input type="hidden" name="cdoid" id="cdoid" value="">
											<input type="hidden" name="prcidCreditoOrc" id="prcidCreditoOrc" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidCreditoOrc',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
													
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodCreditoOrc', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$cdovalor = $_POST['cdovalor']; 
														echo campo_texto( 'cdovalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='cdovalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarCreditoOrc" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('cdo','CreditoOrc')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyCreditoOrc">
											<?php 
												$sql = "select co.cdoid, nd.ndpcod, co.cdovalor from academico.creditoorcamentario co
    														inner join public.naturezadespesa nd on co.ndpid = nd.ndpid
    													where co.prcid = {$_SESSION['academico']['prcid']}";
												$arCreditoOrcamentario = $db->carregar($sql);
												$arCreditoOrcamentario = ($arCreditoOrcamentario) ? $arCreditoOrcamentario : array();
												$i = 0;
												foreach ($arCreditoOrcamentario as $creditoOrcamentario):
											?>
											<tr id="trCreditoOrc_<?php echo $i;?>">
												<td><?php echo $creditoOrcamentario['ndpcod']; ?></td>
												<td><?php echo number_format($creditoOrcamentario['cdovalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $creditoOrcamentario['cdoid']; ?>" class="alterarCreditoOrc">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $creditoOrcamentario['cdoid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirCreditoOrc">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>
						<tr>
							<td width="150px"><strong>Total:</strong></td>
							<td id="tdTotalCreditoOrc"></td>
							<td></td>
						</tr>						
					</tbody>
				</table>
			</td>
			<!-- DESPESA ORÇAMENTÁRIA -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollDespesaOrc" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formDespesaOrc" name="formDespesaOrc" method="post" action="">
											<input type="hidden" name="rowIndexDespesaOrc" id="rowIndexDespesaOrc" value="">
											<input type="hidden" name="dpoid" id="dpoid" value="">
											<input type="hidden" name="prcidDespesaOrc" id="prcidDespesaOrc" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidDespesaOrc',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodDespesaOrc', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$dpovalor = $_POST['dpovalor']; 
														echo campo_texto( 'dpovalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='dpovalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarDespesaOrc" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('dpo','DespesaOrc')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyDespesaOrc">
											<?php 
												$sql = "select deo.dpoid, nd.ndpcod, deo.dpovalor from academico.despesaorcamentaria deo
    														inner join public.naturezadespesa nd on deo.ndpid = nd.ndpid
    													where deo.prcid = {$_SESSION['academico']['prcid']}";
												$arDespesaOrcamentaria = $db->carregar($sql);
												$arDespesaOrcamentaria = ($arDespesaOrcamentaria) ? $arDespesaOrcamentaria : array();
												$i = 0;
												foreach ($arDespesaOrcamentaria as $despesaOrcamentaria):
											?>
											<tr id="trDespesaOrc_<?php echo $i;?>">
												<td><?php echo $despesaOrcamentaria['ndpcod']; ?></td>
												<td><?php echo number_format($despesaOrcamentaria['dpovalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $despesaOrcamentaria['dpoid']; ?>" class="alterarDespesaOrc">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $despesaOrcamentaria['dpoid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirDespesaOrc">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalDespesaOrc"></td>
						<td></td>
					</tr>
				</table>
			</td>
			<td>
			<?php 
				$tmcid = $_SESSION['academico']['tmcid'];
				if( $tmcid )
				{
					$docid = tcPegarDocid($tmcid);
					
					if($docid)
					{ 
						wf_desenhaBarraNavegacao( $docid , array( 'tmcid' => $tmcid ) );
					}				
				}
				?> 
			</td>
		</tr>
		<!-- RECURSO FINANCEIRO -->		
		<tr>
			<td class="SubTituloDireita" style="text-align:center"><strong>RECURSO FINANCEIRO</strong></td>
			<td class="SubTituloDireita" style="text-align:center" colspan="2"><strong>DESPESA FINANCEIRA</strong></td>
		</tr>
		<tr>
			<td style="text-align:center"><strong>Transferidos pelo Concedente</strong></td>
			<td style="text-align:center"><strong>Com Recursos do Concedente</strong></td>
		</tr>
		<tr>
			<!-- RECURSO FINANCEIRO CONCEDENTE -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollRecursoFinConc" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formRecursoFinConc" name="formRecursoFinConc" method="post" action="">
											<input type="hidden" name="rowIndexRecursoFinConc" id="rowIndexRecursoFinConc" value="">
											<input type="hidden" name="rfcid" id="rfcid" value="">
											<input type="hidden" name="prcidRecursoFinConc" id="prcidRecursoFinConc" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidRecursoFinConc',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodRecursoFinConc', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$rfcvalor = $_POST['rfcvalor']; 
														echo campo_texto( 'rfcvalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='rfcvalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarRecursoFinConc" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('rfc','RecursoFinConc')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyRecursoFinConc">
											<?php 
												$sql = "select rfc.rfcid, nd.ndpcod, rfc.rfcvalor from academico.recursofinanceiroconcedente rfc
    														inner join public.naturezadespesa nd on rfc.ndpid = nd.ndpid
    													where rfc.prcid = {$_SESSION['academico']['prcid']} ";
												$arRecursoFinanceiroConcedente = $db->carregar($sql);
												$arRecursoFinanceiroConcedente = ($arRecursoFinanceiroConcedente) ? $arRecursoFinanceiroConcedente : array();
												$i = 0;
												foreach ($arRecursoFinanceiroConcedente as $recursoFinanceiroConcedente):
											?>
											<tr id="trRecursoFinConc_<?php echo $i;?>">
												<td><?php echo $recursoFinanceiroConcedente['ndpcod']; ?></td>
												<td><?php echo number_format($recursoFinanceiroConcedente['rfcvalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $recursoFinanceiroConcedente['rfcid']; ?>" class="alterarRecursoFinConc">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $recursoFinanceiroConcedente['rfcid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirRecursoFinConc">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalRecursoFinConc"></td>
						<td></td>
					</tr>
				</table>
			</td>
			<!-- DESPESA FINANCEIRA CONCEDENTE -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollDespFinanConc" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formDespFinanConc" name="formDespFinanConc" method="post" action="">
											<input type="hidden" name="rowIndexDespFinanConc" id="rowIndexDespFinanConc" value="">
											<input type="hidden" name="dfcid" id="dfcid" value="">
											<input type="hidden" name="prcidDespFinanConc" id="prcidDespFinanConc" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidDespFinanConc',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodDespFinanConc', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$dfcvalor = $_POST['dfcvalor']; 
														echo campo_texto( 'dfcvalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='dfcvalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarDespFinanConc" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('dfc','DespFinanConc')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyDespFinanConc">
											<?php 
												$sql = "select dfc.dfcid, nd.ndpcod, dfc.dfcvalor from academico.despesafinanceiraconcedente dfc
    														inner join public.naturezadespesa nd on dfc.ndpid = nd.ndpid
    													where dfc.prcid = {$_SESSION['academico']['prcid']} ";
												$arDespesaFinanceiraConcedente = $db->carregar($sql);
												$arDespesaFinanceiraConcedente = ($arDespesaFinanceiraConcedente) ? $arDespesaFinanceiraConcedente : array();
												$i = 0;
												foreach ($arDespesaFinanceiraConcedente as $despesaFinanceiraConcedente):
											?>
											<tr id="trDespFinanConc_<?php echo $i;?>">
												<td><?php echo $despesaFinanceiraConcedente['ndpcod']; ?></td>
												<td><?php echo number_format($despesaFinanceiraConcedente['dfcvalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $despesaFinanceiraConcedente['dfcid']; ?>" class="alterarDespFinanConc">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $despesaFinanceiraConcedente['dfcid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirDespFinanConc">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalDespFinanConc"></td>
						<td></td>
					</tr>
				</table>
			</td>
		</tr>
		
		<tr>
			<td style="text-align:center"><strong>Com Recursos Próprios</strong></td>
			<td style="text-align:center"><strong>Com Recursos Próprios</strong></td>
		</tr>
		<tr>
			<!-- RECURSO FINANCEIRO PRÓPRIO -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollRecursoFinanPro" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formRecursoFinanPro" name="formRecursoFinanPro" method="post" action="">
											<input type="hidden" name="rowIndexRecursoFinanPro" id="rowIndexRecursoFinanPro" value="">
											<input type="hidden" name="rfpid" id="rfpid" value="">
											<input type="hidden" name="prcidRecursoFinanPro" id="prcidRecursoFinanPro" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidRecursoFinanPro',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodRecursoFinanPro', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$rfpvalor = $_POST['rfpvalor']; 
														echo campo_texto( 'rfpvalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='rfpvalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarRecursoFinanPro" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('rfp','RecursoFinanPro');"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyRecursoFinanPro">
											<?php 
												$sql = "select rfp.rfpid, nd.ndpcod, rfp.rfpvalor from academico.recursofinanceiroproprio rfp
    														inner join public.naturezadespesa nd on rfp.ndpid = nd.ndpid
    													where rfp.prcid = {$_SESSION['academico']['prcid']} ";
												$arRecursoFinanceiroProprio = $db->carregar($sql);
												$arRecursoFinanceiroProprio = ($arRecursoFinanceiroProprio) ? $arRecursoFinanceiroProprio : array();
												$i = 0;
												foreach ($arRecursoFinanceiroProprio as $recursoFinanceiroProprio):
											?>
											<tr id="trRecursoFinanPro_<?php echo $i;?>">
												<td><?php echo $recursoFinanceiroProprio['ndpcod']; ?></td>
												<td><?php echo number_format($recursoFinanceiroProprio['rfpvalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $recursoFinanceiroProprio['rfpid']; ?>" class="alterarRecursoFinanPro">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $recursoFinanceiroProprio['rfpid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirRecursoFinanPro">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalRecursoFinanPro"></td>
						<td></td>
					</tr>
				</table>
			</td>
			<!-- DESPESA FINANCEIRA PRÓPRIO -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollDespesaFinanPro" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formDespesaFinanPro" name="formDespesaFinanPro" method="post" action="">
											<input type="hidden" name="rowIndexDespesaFinanPro" id="rowIndexDespesaFinanPro" value="">
											<input type="hidden" name="dfpid" id="dfpid" value="">
											<input type="hidden" name="prcidDespesaFinanPro" id="prcidDespesaFinanPro" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidDespesaFinanPro',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodDespesaFinanPro', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$dfpvalor = $_POST['dfpvalor']; 
														echo campo_texto( 'dfpvalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='dfpvalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarDespesaFinanPro" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('dfp','DespesaFinanPro')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyDespesaFinanPro">
											<?php 
												$sql = "select dfp.dfpid, nd.ndpcod, dfp.dfpvalor from academico.despesafinanceiraproprio dfp
    														inner join public.naturezadespesa nd on dfp.ndpid = nd.ndpid
    													where dfp.prcid = {$_SESSION['academico']['prcid']} ";
												$arDespesaFinanceiraProprio = $db->carregar($sql);
												$arDespesaFinanceiraProprio = ($arDespesaFinanceiraProprio) ? $arDespesaFinanceiraProprio : array();
												$i = 0;
												foreach ($arDespesaFinanceiraProprio as $despesaFinanceiraProprio):
											?>
											<tr id="trDespesaFinanPro_<?php echo $i;?>">
												<td><?php echo $despesaFinanceiraProprio['ndpcod']; ?></td>
												<td><?php echo number_format($despesaFinanceiraProprio['dfpvalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $despesaFinanceiraProprio['dfpid']; ?>" class="alterarDespesaFinanPro">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $despesaFinanceiraProprio['dfpid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirDespesaFinanPro">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalDespesaFinanPro"></td>
						<td></td>
					</tr>
				</table>
			</td>
		</tr>
		
		
		
		<tr>
			<td style="text-align:center"><strong>Devolução Recurso</strong></td>
			<td style="text-align:center"><strong>Devolução Despesa</strong></td>
		</tr>
		<tr>
			<!-- DEVOLUÇÃO RECURSO -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollDevolucaoRecur" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formDevolucaoRecur" name="formDevolucaoRecur" method="post" action="">
											<input type="hidden" name="rowIndexDevolucaoRecur" id="rowIndexDevolucaoRecur" value="">
											<input type="hidden" name="derid" id="derid" value="">
											<input type="hidden" name="prcidDevolucaoRecur" id="prcidDevolucaoRecur" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidDevolucaoRecur',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodDevolucaoRecur', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$dervalor = $_POST['dervalor']; 
														echo campo_texto( 'dervalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='dervalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarDevolucaoRecur" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('der','DevolucaoRecur');"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyDevolucaoRecur">
											<?php 
												$sql = "select der.derid, nd.ndpcod, der.dervalor from academico.devolucaorecurso der
    														inner join public.naturezadespesa nd on der.ndpid = nd.ndpid
    													where der.prcid = {$_SESSION['academico']['prcid']} ";
												$arDevolucaoRecurso = $db->carregar($sql);
												$arDevolucaoRecurso = ($arDevolucaoRecurso) ? $arDevolucaoRecurso : array();
												$i = 0;
												foreach ($arDevolucaoRecurso as $devolucaoDespesa):
											?>
											<tr id="trDevolucaoRecur_<?php echo $i;?>">
												<td><?php echo $devolucaoDespesa['ndpcod']; ?></td>
												<td><?php echo number_format($devolucaoDespesa['dervalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $devolucaoDespesa['derid']; ?>" class="alterarDevolucaoRecur">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $devolucaoDespesa['derid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirDevolucaoRecur">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalDevolucaoRecur"></td>
						<td></td>
					</tr>
				</table>
			</td>
			
			<!-- DEVOLUÇÃO DESPESA -->
			<td align="center">
				<table style="width: 57%; border-style:solid; border-width:1px" >
					<tbody>
						<tr>
							<td colspan="3">
								<div class="scrollDevolucaoDesp" >
									<table style="width: 100%; cellpadding: 0; cellspacing: 0 ">
										<thead>
											<tr style="background-color: #e0e0e0">
												<th style="font-weight:bold; text-align:center; width:50%">Natureza</th>
												<th style="font-weight:bold; text-align:center; width:30%">Valor</th>
												<th style="font-weight:bold; text-align:center; width:20%">Ações</th>
											</tr>
											<form id="formDevolucaoDesp" name="formDevolucaoDesp" method="post" action="">
											<input type="hidden" name="rowIndexDevolucaoDesp" id="rowIndexDevolucaoDesp" value="">
											<input type="hidden" name="dedid" id="dedid" value="">
											<input type="hidden" name="prcidDevolucaoDesp" id="prcidDevolucaoDesp" value="<?php echo $obProcesso->prcid;?>">
											<tr style="background-color: #e0e0e0" height="10px">
												<th align="center">
													<?php
														/*$sql = "select ndpid as value, ndpcod as codigo, ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														campo_popup('ndpidDevolucaoDesp',$sql,'Selecione','','400x800','20', array(
																													array("codigo" => "ndpcod",
																		  												  "descricao" => "<b>Código:</b>","numeric" => "1"),
																													array("codigo" => "ndpdsc",
																														  "descricao" => "<b>Descrição:</b>","string" => "1")
																													), 1, false, $habilitaComboPopup);*/
														$sql = "select ndpcod as codigo, 
															    	   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
														texto_popup( 'ndpcodDevolucaoDesp', $sql, 'Natureza', 8, 20, '########', '', $complemento );
													?>
												</th>
												<th align="center">
													<?php 
														$dedvalor = $_POST['dedvalor']; 
														echo campo_texto( 'dedvalor', 'N', $habilitado, '', 12, 20, '###.###.###.###,##', '', '', '','', "id='dedvalor'" ); 
													?>
												</th>
												<th align="center">
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/gif_inclui.gif" id="salvarDevolucaoDesp" style="cursor: pointer;" border=0 title="Salvar" />
													<?php else:?>
														<img src="/imagens/gif_inclui_d.gif" border=0 title="Salvar" />
													<?php endif;?>
													&nbsp; <a href="javascript: void(0);" onclick="limparCampos('ded','DevolucaoDesp')"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a>
												</th>
											</tr>
											</form>
										</thead>
										<tbody id="tbodyDevolucaoDesp">
											<?php 
												$sql = "select ded.dedid, nd.ndpcod, ded.dedvalor from academico.devolucaodespesa ded
    														inner join public.naturezadespesa nd on ded.ndpid = nd.ndpid
    													where ded.prcid = {$_SESSION['academico']['prcid']} ";
												$arDevolucaoDespesa = $db->carregar($sql);
												$arDevolucaoDespesa = ($arDevolucaoDespesa) ? $arDevolucaoDespesa : array();
												$i = 0;
												foreach ($arDevolucaoDespesa as $devolucaoDespesa):
											?>
											<tr id="trDevolucaoDesp_<?php echo $i;?>">
												<td><?php echo $devolucaoDespesa['ndpcod']; ?></td>
												<td><?php echo number_format($devolucaoDespesa['dedvalor'],2,',','.'); ?></td>
												<td>
													<?php if($habilitado == 'S'): ?>
														<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $devolucaoDespesa['dedid']; ?>" class="alterarDevolucaoDesp">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $devolucaoDespesa['dedid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirDevolucaoDesp">
													<?php else:?>
														<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
													<?php endif;?>
												</td>
											</tr>
											<?php $i++;?>
											<?php endforeach; ?>
										</tbody>
									</table>
								</div>
							</td>
						</tr>						
					</tbody>
					<tr>
						<td width="150px"><strong>Total:</strong></td>
						<td id="tdTotalDevolucaoDesp"></td>
						<td></td>
					</tr>
				</table>
			</td>
		</tr>
		
		
	</tbody>
</table>
<style type="text/css">
div.scrollCreditoOrc {
	width: 100%;
	height: 170px;
}

div.scrollDespesaOrc {
	width: 100%;
	height: 170px;
}

div.scrollRecursoFinConc {
	width: 100%;
	height: 170px;
}

div.scrollDespFinanConc {
	width: 100%;
	height: 170px;
}

div.scrollRecursoFinanPro {
	width: 100%;
	height: 170px;
}

div.scrollDespesaFinanPro {
	width: 100%;
	height: 170px;
}

div.scrollDevolucaoRecur {
	width: 100%;
	height: 170px;
}

div.scrollDevolucaoDesp {
	width: 100%;
	height: 170px;
}
</style>
<div id="divDebug"></div>