<input type="hidden" name="req" value=""/>
<input type="hidden" name="tipoSalvar" value="resposta"/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=1";
			$param['link2'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=2";
			$param['link2tipo'] = 2;
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
				usucpf
			FROM 
				academico.pergunta per
			LEFT JOIN academico.resposta res ON res.perid = per.perid AND entid = ".$_SESSION['academico']['entidcampus']."
			WHERE
				pernumero ilike '2.4.%'
				OR pernumero ilike '2.5.%'
				OR pernumero ilike '2.6.%'
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
			<b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b>
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
			$cond4['teste'] = $dado['percondicao'] == 'S' ? true : false;
			$cond4['func']  = $dado['percondicao'] == 'S' ? 'monstraCondicao('.$dado['perid'].', this.value)' : '';
			$cond4['valor'] = $dado['resdescricao'];
			$cond4['perid'] = $dado['perid'];
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
			<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="resdescricao[<?=$dado['perid'] ?>]" onclick="<?=$cond4['func'] ?>" value="TRUE" <?=($dado['resdescricao'] == 'TRUE' ? 'checked="checked"' : '')  ?>/> Sim
			<input <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>type="radio" name="resdescricao[<?=$dado['perid'] ?>]" onclick="<?=$cond4['func'] ?>" value="FALSE" <?=($dado['resdescricao'] == 'FALSE' ? 'checked="checked"' : '') ?>/> Não
<!--			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">-->
			<?=campo_textarea('resjustificativa['.$dado['perid'].']', 'N', $bloq, '', 80, 5, $dado['pertamanho'],'','','','','',$dado['resjustificativa']) ?>
		</td>
	</tr>
	<?php 
		}
		if( count($num) == 5 ){
			$display = '';
			if( $cond4['teste'] ){
				if( $cond4['valor'] != 'TRUE' ){
					$display = 'display:none;';
				}
			}
	?>
	<tr style="<?=$display ?>" class="hidden<?=$cond4['perid'] ?>">
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td colspan="2" style="font-size:12px;" ><b><?=$dado['pernumero']?>  <?=$dado['pertitulo'] ?></b></td>
	</tr>
	<tr style="<?=$display ?>" class="hidden<?=$cond4['perid'] ?>">
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td width="30px;"></td>
		<td>
			<input type="radio" <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>name="resdescricao[<?=$dado['perid'] ?>]" value="TRUE" <?=($dado['resdescricao'] == 'TRUE' ? 'checked="checked"' : '')  ?>/> Sim
			<input type="radio" <?=($bloq == 'S' ? '' : 'disabled="disabled"') ?>name="resdescricao[<?=$dado['perid'] ?>]" value="FALSE" <?=($dado['resdescricao'] == 'FALSE' ? 'checked="checked"' : '') ?>/> Não
			<?php $param['id'] = 'resjustificativa_'.$dado['perid']; ?>
			<?=campo_textarea('resjustificativa['.$dado['perid'].']', 'N', $bloq, '', 80, 5, $dado['pertamanho'],'','','','','',$dado['resjustificativa'],$param) ?>
		</td>
	</tr>
	<?php 	
		}
	}
	?>
</table>
<script>
function monstraCondicao( perid, valor ){

	if( valor == 'TRUE' ){
		$('.hidden'+perid).show();
	}else{
		$('.hidden'+perid).hide();
	}
	
}
</script>