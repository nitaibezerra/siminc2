<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "par/classes/controle/CampoExternoControle.class.inc";
include_once APPRAIZ . "par/classes/modelo/QuestoesPontuaisAnexos.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include  	 APPRAIZ . "includes/cabecalho.inc";


//$qrpid = 10148375;
$qrpid = 10153951;
?>

</script>
<br />
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'relatorio' => 'modulo=relatorio/relatorioQuestionario&acao=A', 'habilitado' => $habilitado  ) );
			?>
		</fieldset>
	</tr>
</table>
