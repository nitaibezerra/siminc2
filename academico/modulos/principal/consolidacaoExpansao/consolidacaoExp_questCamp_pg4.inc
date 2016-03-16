<input type="hidden" name="req" value=""/>
<input type="hidden" name="tipoSalvar" value="justivifativa"/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=4";
			montarAbasConsolidacao( $param );
			?>
		</td>
	</tr>
	<?php 
	//Inicio Lista Questões
	$sql = "SELECT 
				per.perid,
				tppid, 
				pernumero, 
				pertitulo, 
				pernivel, 
				perordem, 
				perjutificativa, 
				pertamanho, 
				percondicao,
				resdescricao,
				resjustificativa,
				res.usucpf,
				arqid
			FROM 
				academico.pergunta per
			LEFT JOIN academico.anexosquestionario 	aqe ON aqe.perid = per.perid AND aqe.entid = ".$_SESSION['academico']['entidcampus']."
			LEFT JOIN academico.resposta 			res ON res.perid = per.perid AND res.entid = ".$_SESSION['academico']['entidcampus']."
			WHERE
				pernumero ilike '3.%' OR pernumero = ''
			ORDER BY
				1";
	$dados = $db->carregar($sql);
	foreach($dados as $dado){
		$num = explode('.',$dado['pernumero']);
		if( count($num) == 2 ){
	?>
	<tr>
		<td width="30px;"></td>
		<td colspan="4" style="font-size:12px;" ><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b></td>
	</tr>
	<?php 
		}
		if( count($num) == 3 ){
			$arquivo = '';
			if( $dado['tppid'] == 5  ){
				if( $dado['arqid'] ){
					if( $bloq == 'S' ){
						$arquivo = '<a class="baixar" style="cursor:pointer" id="'.$dado['arqid'].'">Clique aqui para baixar</a>
									<img border="0" title="Excluir arquivo." style="cursor:pointer" width="12px" class="excluir" id="'.$dado['arqid'].'" src="../imagens/excluir_2.gif">';
					}else{
						$arquivo = '<a class="baixar" style="cursor:pointer" id="'.$dado['arqid'].'">Clique aqui para baixar</a>';
					}
				}else{
					$arquivo = '<input type="file" name="arquivo_'.$dado['perid'].'" />';
				}
			}
	?>
	<tr>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td colspan="3" style="font-size:12px;" ><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b> <?=$arquivo ?></td>
	</tr>
	<?php 
		}
		if( $dado['pernumero'] == '' ){
//			if( $dado['pertitulo'] == '1º' ){
//				$obrig = 'S';
//			}else{
				$obrig = 'N';
//			}
	?>
	<tr>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b></td>
		<td>
			<?php $param['id'] = 'resjustificativa_'.$dado['perid']; ?>
			<?=campo_textarea('resjustificativa['.$dado['perid'].']', $obrig, $bloq, '', 120, 1, $dado['pertamanho'],'','','','','',$dado['resjustificativa'],$param) ?>
		</td>
	</tr>
	<?php 
		}
	}
	?>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$('.excluir').live('click',function(){
	window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&req=excluirArquivo&arqid='+$(this).attr('id');
});
$('.baixar').live('click',function(){
	return window.open('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&arqid='+$(this).attr('id'), 
			   'ajuda', 
			   "height=300,width=450,scrollbars=yes,top=50,left=200" );
});
</script>