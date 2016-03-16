<?php

extract($_POST);

$aWhere = array();
if ($projeto) {
	$aWhere[] = "a1.atidescricao ilike ('%$projeto%')"; 
}
if ($atidescricao) {
	$aWhere[] = "a2.atidescricao ilike ('%$atidescricao%')"; 
}
if ($subatividade) {
	$aWhere[] = "a3.atidescricao ilike ('%$subatividade%')"; 
}
if ($mnmdsc) {
	$aWhere[] = "mnm.mnmdsc ilike ('%$mnmdsc%')"; 
}
if (isset($micestrategico)) {
	$aWhere[] = "mic.micestrategico = 't'"; 
}
if (isset($micenviasms)) {
	$aWhere[] = "mic.micenviasms = 't'";
}

$sql = "SELECT distinct 
			a1.atiid,
			a1._atinumero as _atinumero1,	
			a1.atidescricao as projeto, 
			a2.atiid,
			a2._atinumero as _atinumero2,	
			a2.atidescricao, 
			a3.atiid,
			a3._atinumero as _atinumero3,	
			a3.atidescricao as subatividade, 	
			dmidatameta,
			to_char(dmidatameta, 'DD/MM/YYYY') as data_meta,
			dmiid,
			i.metid,
			mic.micid,
			a1.atiid,
			mic.indid,
			mnm.mnmid,
			mnm.mnmdsc,
			mnm.micid,
			mnm.metid,
			CASE WHEN mic.micestrategico = 't' THEN 'Sim' ELSE 'Não' END as critico,
			CASE WHEN mic.micenviasms    = 't' THEN 'Sim' ELSE 'Não' END as enviasms			
		FROM pde.atividade a1
			INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
			INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
			INNER JOIN pde.monitoraitemchecklist mic on mic.atiid = a3.atiid AND mic.micstatus = 'A'
			INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
			INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
			INNER JOIN workflow.documento doc ON doc.docid = i.docid 
		WHERE doc.esdid = 443 
		and i.dmidatameta < CURRENT_DATE
		 -- and a1.atiid = 144340 
		AND a1.atistatus = 'A'";

$sql .= count($aWhere) ? ' AND ' . implode(" AND ", $aWhere) : ''; 

$sql .= "order by a1.atidescricao, a2._atinumero, a3._atinumero, mic.indid";


$aPendencias = $db->carregar($sql);
//ver(!$aPendencias, count((array)$aPendencias), $sql, d);

//ver(count($aPendencias), $aPendencias);

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Metas de Checklist Pendentes', '' );


?>

<form method="post" name="pesquisarProjeto" id="pesquisarProjeto" >
	<input type="hidden" id="requisicao" name="requisicao" value="pesquisarProjeto"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Projeto:</td>
			<td>
				<?php echo campo_texto('projeto','N',$permissao,'',60, 500,'','',null,null,null,'id="projeto"'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Atividade:</td>
			<td>
				<?php echo campo_texto('atidescricao','N',$permissao,'',60, 500,'','',null,null,null,'id="atidescricao"'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Sub-atividade:</td>
			<td>
				<?php echo campo_texto('subatividade','N',$permissao,'',60, 500,'','',null,null,null,'id="subatividade"'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td>
				<?php echo campo_texto('mnmdsc','N',$permissao,'',60, 500,'','',null,null,null,'id="mnmdsc"'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Crítico:</td>
			<td>
				<input type='checkbox' name='micestrategico' id='micestrategico' <?php  echo $micestrategico ? 'checked' : ''; ?>>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Envia SMS:</td>
			<td>
				<input type='checkbox' name='micenviasms' id='micenviasms' <?php  echo $micenviasms ? 'checked' : '' ?>>
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#c5c5c5" colspan="2" >
				<input type="submit" name="btn_pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<?php if ($aPendencias) : ?>
		<thead>
			<tr bgcolor="#dfdfdf">
				<td align="center"><b>Projeto</b></td>
				<td align="center"><b>Atividade</b></td>
				<td align="center"><b>Sub-atividade</b></td>
				<td align="center"><b>Descrição</b></td>
				<td align="center"><b>Crítico</b></td>
				<td align="center"><b>Envia SMS</b></td>
				<td align="center"><b>Executor</b></td>
				<td align="center"><b>Meta</b></td>
			</tr>
		</thead>
		<?php $cor = ''; ?>
		<?php foreach ( $aPendencias as $pendencia ) : ?>
			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; 
			
			$aExecutores = getExecutoresPorMnmid($pendencia['mnmid']);
			$aExecutor = array();
			foreach ($aExecutores as $executor) {
				$telefone = $executor['celular_executor'] ? $executor['celular_executor'] : 'Sem Telefone Cadastrado';
				$email   = $executor['entemail'] ? $executor['entemail'] : 'Sem Email Cadastrado';
				$aExecutor[] = '<span title="' . $telefone . ' - ' . $email . '">' . $executor['entnome'] . '</span>';
			}
			?>
			
			<tr
				bgcolor="<?= $cor ?>"
				onmouseout="this.style.backgroundColor='<?= $cor ?>';"
				onmouseover="this.style.backgroundColor='#ffffcc';"
			>
				<td><span><?= $pendencia['projeto'] ?></span></td>
				<td><span><?= $pendencia['_atinumero2'] . ' - ' . $pendencia['atidescricao'] ?></span></td>
				<td><span><?= $pendencia['_atinumero3'] . ' - ' . $pendencia['subatividade'] ?></span></td>
				<td><span><?= $pendencia['mnmdsc'] ?></span></td>
				<td align="center" ><span><?=$pendencia['critico'] ?></span></td>
				<td align="center" ><span><?=$pendencia['enviasms'] ?></span></td>
				<td><span><?= implode('<br />', $aExecutor); ?></span></td>
				<td align="center" ><span ><?=$pendencia['data_meta'] ?></span></td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
				Não há pendências com os filtros informados
			</td>
		</tr>
	<?php endif; ?>
</table>

<?php 
function getExecutoresPorMnmid($mnmid)
{
	global $db;
	$sql = "select distinct ent.entid, ent.entnome, ent.entemail, '(' || ent.entnumdddcelular || ') ' ||ent.entnumcelular as celular_executor
			from  pde.monitorametaentidade me
				inner join entidade.entidade ent ON ent.entid = me.entid
			where me.mnmid = '$mnmid'
			and me.tpvid = 1";
	
	return $db->carregar($sql);	
}
?>