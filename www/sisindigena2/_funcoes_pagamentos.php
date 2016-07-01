<?
function autorizarPagamentos($dados) {
	global $db;
	
	$_TPD_AUTORIZACAO = array(ESD_PAGAMENTO_APTO => AED_AUTORIZAR_APTO,ESD_PAGAMENTO_RECUSADO => AED_AUTORIZAR_RECUSADO);
	
	
	if($dados['pboid']) {
		foreach($dados['pboid'] as $pboid) {
			
			$pagamento = $db->pegaLinha("SELECT d.docid, d.esdid FROM sisindigena2.pagamentobolsista p 
										 INNER JOIN workflow.documento d ON d.docid = p.docid 
										 WHERE pboid='".$pboid."'");
			
			if($_TPD_AUTORIZACAO[$pagamento['esdid']]) {
				wf_alterarEstado( $pagamento['docid'], $_TPD_AUTORIZACAO[$pagamento['esdid']], $cmddsc = '', array());
			}
			
		}
	}
	
	$al = array("alert"=>"Pagamentos autorizados com sucesso","javascript"=>"window.opener.location=window.opener.location;window.close();");
	alertlocation($al);
	
	
}

function cancelarPagamento($dados) {
	global $db;
	
	$pagamentobolsista = $db->pegaLinha("SELECT * FROM sisindigena2.pagamentobolsista WHERE pboid='".$dados['pboid']."'");
	
	$db->executar("DELETE FROM sisindigena2.pagamentobolsista WHERE pboid='".$dados['pboid']."'");
	
	$db->executar("DELETE FROM sisindigena2.mensarioavaliacoes WHERE menid IN(SELECT menid FROM sisindigena2.mensario WHERE iusd='".$pagamentobolsista['iusd']."' AND fpbid='".$pagamentobolsista['fpbid']."')");
	
	$db->commit();
	
	wf_alterarEstado( $pagamentobolsista['docid'], AED_REABRIR_MENSARIO_APROVADO, $dados['justificativa'], array());
	
	
}

?>