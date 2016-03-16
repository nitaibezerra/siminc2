<?
function autorizarPagamentos($dados) {
	global $db;
	
	$_TPD_AUTORIZACAO = array(ESD_PAGAMENTO_APTO => AED_AUTORIZAR_APTO,ESD_PAGAMENTO_RECUSADO => AED_AUTORIZAR_RECUSADO);
	
	
	if($dados['pboid']) {
		foreach($dados['pboid'] as $pboid) {
			
			$pagamento = $db->pegaLinha("SELECT d.docid, d.esdid FROM escolaterra.pagamentobolsista p 
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

function aprovarTrocaNomesSGB($dados) {
	global $db;

	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			if($dados['nome_receita'][$cpf]) {
				$sql = "UPDATE escolaterra.identificacaousuario SET iusnome='".$dados['nome_receita'][$cpf]."' WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'";
				$db->executar($sql);
			}
		}
		$db->commit();
	}

	$al = array("alert"=>"Nome atualizados com sucesso","location"=>"escolaterra.php?modulo=principal/pagamentos/aprovartrocanomes&acao=A");
	alertlocation($al);

}


?>