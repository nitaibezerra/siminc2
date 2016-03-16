<input type="hidden" name="req" value=""/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=12";
			montarAbasConsolidacao( $param );
			?>
		</td>
	</tr>
	<tr>
		<td style="font-size:12px;" >
<!--			<b>5. Demanda - Arquivos</b>-->
		</td>
	</tr>
	<?php 
	//Inicio Lista Questões
	$sql = "SELECT 
				perid, 
				pernumero, 
				pertitulo
			FROM 
				academico.pergunta 
			WHERE
				pernumero ilike '4.3.%'";
	
	$item = $db->pegaLinha($sql);
	?>
	<tr>
		<td style="font-size:12px;" >
			<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' style="width:100%;" >
				<tr>
					<td bgcolor="#DCDCDC" align="center" width="80%"><b>Pendências</b></td>
					<td align="center" rowspan="100" valign="top">
						<?php 
						require_once APPRAIZ . 'includes/workflow.php';
						$docid = pegaDocidConsolidacao();
						wf_desenhaBarraNavegacao( $docid , 
												  array( 'entid' => $_SESSION['academico']['entid'], 'entidcampus' => $_SESSION['academico']['entidcampus'] ), 
												  array('historico'=>true) );
						?>
					</td>
				</tr>
				<tr>
					<td>
						<?php 
							verificaPreenchimentoQuestionario2( $_SESSION['academico']['entidcampus'] );
						?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$('.ir').live('click',function(){
	window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg='+$(this).attr('id');
});
$('.irUni').live('click',function(){
	window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A';
});
</script>