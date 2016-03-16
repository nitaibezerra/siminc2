<?

$avaliacaofinalcg = $db->pegaLinha("SELECT * FROM sismedio.avaliacaofinalcg WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'");
if($avaliacaofinalcg) {
	extract($avaliacaofinalcg);
	
	if(!$docid) {
		$docid = wf_cadastrarDocumento(TPD_FLUXORELATORIOFINAL,"Relatório Final SISMédio ".$_SESSION['sismedio']['universidade']['iusd']);
		$db->executar("UPDATE sismedio.avaliacaofinalcg SET docid='{$docid}' WHERE afcid='{$afcid}'");
		$db->commit();
	}
	
	$estado = wf_pegarEstadoAtual( $docid );
	
	if($estado['esdid'] != ESD_RELATORIOFINAL_EMELABORACAO) {
		$consulta = true;
	}
	
}

$es = estruturaAvaliacaoCG(array('consulta'=>$consulta));
			
$titulo 		 = 'Relatório final da Formação – Coordenador-Geral da IES';

echo '<form method="post" id="formulario" enctype="multipart/form-data">';
echo '<input type="hidden" name="requisicao" value="gravarAvaliacaoFinalCG">';

echo '<table width=100%>';
echo '<tr>';
echo '<td>';

include_once APPRAIZ_SISMEDIO."/montarQuestionario.inc";

echo '</td>';

echo '<td width=2% valign=top>';

wf_desenhaBarraNavegacao( $docid, array() );

echo '</td>';

echo '</tr>';
echo '</table>';

echo '</form>';

if($consulta) echo '<script>jQuery(\'#salvar\').remove();</script>';
			
			


?>