<?php 
//$sql = "SELECT
//			resdescricao
//		FROM 
//			academico.pergunta per
//		LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = ".$_SESSION['academico']['entid']."
//		WHERE
//			pernumero = '1.'";
//$resp = $db->pegaUm($sql);
//if( !$resp ){
?>
<script>
//	window.location = window.location = '?modulo=principal/consolidacaoExpansao/consolidacaoExp_questUni&acao=A&entid=' + <?=$_SESSION['academico']['entid'] ?>;
</script>
<?php 
//}
?>
<input type="hidden" name="req" value=""/>
<input type="hidden" name="tipoSalvar" value="resposta"/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
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
				usucpf
			FROM 
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = ".$_SESSION['academico']['entidcampus']."
			WHERE
				pernumero ilike '2.'
				OR pernumero ilike '2.1.%'
				OR pernumero ilike '2.2.%'
				OR pernumero ilike '2.3.%'
			ORDER BY
				3";
	$dados = $db->carregar($sql);
	foreach($dados as $dado){
		$num = explode('.',$dado['pernumero']);
		if( count($num) == 2 ){
	?>
	<tr>
		<td width="30px;"></td>
		<td colspan="4" style="font-size:12px;" >
		
<!--			<b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b>-->
		</td>
	</tr>
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=1";
			$param['link2'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=1";
			$param['link2tipo'] = 2;
			montarAbasConsolidacao( $param );
			?>
		</td>
	</tr>
	<?php 
		}
		if( count($num) == 3 ){
	?>
	<tr>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td colspan="3" style="font-size:12px;" ><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b></td>
	</tr>
	<?php 
		}
		if( count($num) == 4 ){
	?>
	<tr>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td colspan="2" style="font-size:12px;" ><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b></td>
	</tr>
	<tr>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td>
			<input type="radio" name="resdescricao[<?=$dado['perid'] ?>]" value="TRUE" <?=($dado['resdescricao'] == 'TRUE' ? 'checked="checked"' : '')  ?><?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/> Sim
			<input type="radio" name="resdescricao[<?=$dado['perid'] ?>]" value="FALSE" <?=($dado['resdescricao'] == 'FALSE' ? 'checked="checked"' : '') ?><?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>/> Não
<!--			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">-->
			<?php $param['id'] = 'resjustificativa_'.$dado['perid']; ?>
			<?=campo_textarea('resjustificativa['.$dado['perid'].']', 'N', $bloq, '', 80, 5, $dado['pertamanho'],'','','','','',$dado['resjustificativa'],$param) ?>
		</td>
	</tr>
	<?php 
		}
	}
	?>
</table>