<?

function excluirPagamento($dados) {
	global $db;
	
	$sql = "SELECT iusd, fpbid FROM sismedio.pagamentobolsista WHERE pboid='".$dados['pboid']."'";
	$pagamentobolsista = $db->pegaLinha($sql);
	
	$sql = "SELECT menid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid WHERE m.iusd='".$pagamentobolsista['iusd']."' AND m.fpbid='".$pagamentobolsista['fpbid']."' ANd d.esdid='".ESD_APROVADO_MENSARIO."'";
	$menid = $db->pegaUm($sql);
	
	if($menid) {
		$sql = "DELETE FROM sismedio.mensarioavaliacoes WHERE menid='{$menid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM sismedio.mensario WHERE menid='{$menid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM sismedio.pagamentobolsista WHERE pboid='".$dados['pboid']."'";
		$db->executar($sql);
		
		$db->commit();
		
	}
	
}

function autorizarPagamentos($dados) {
	global $db;
	
	$_TPD_AUTORIZACAO = array(ESD_PAGAMENTO_APTO => AED_AUTORIZAR_APTO,ESD_PAGAMENTO_RECUSADO => AED_AUTORIZAR_RECUSADO);
	
	
	if($dados['pboid']) {
		foreach($dados['pboid'] as $pboid) {
			
			$pagamento = $db->pegaLinha("SELECT d.docid, d.esdid FROM sismedio.pagamentobolsista p 
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

?>