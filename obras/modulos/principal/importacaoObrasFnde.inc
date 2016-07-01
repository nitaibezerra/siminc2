<?php 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$titulo1 = "Importação Obras FNDE";
$titulo2 = "Selecione os filtros e agrupadores desejados";
monta_titulo( $titulo1, $titulo2 );

if($_POST) extract($_POST);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btn_pesquisar').click(function(){
			if(!$('[name=tipo]:checked').val()){
				alert('Escola uma tabela de apoio!');
				$('[name=tipo]').focus()
				return false;
			}
			$('#formulario').submit();
		});
		$('#btn_limpar').click(function(){
			document.location.href = 'obras.php?modulo=principal/importacaoObrasFnde&acao=A';
		});
	});
</script>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Descrição</td>
			<td>
				<?php echo campo_texto('descricao', 'N', 'S', '', 55, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tabela de apoio</td>
			<td>
				<input type="radio" name="tipo" value="fonte" <?php echo $tipo == 'fonte' ? 'checked="checked"' : ''; ?>/>&nbsp;Fonte&nbsp;
				<input type="radio" name="tipo" value="tipologia" <?php echo $tipo == 'tipologia' ? 'checked="checked"' : ''; ?>/>&nbsp;Tipologia&nbsp;
				<input type="radio" name="tipo" value="programa" <?php echo $tipo == 'programa' ? 'checked="checked"' : ''; ?>/>&nbsp;Programa&nbsp;
				<input type="radio" name="tipo" value="unidadeimplantadora" <?php echo $tipo == 'unidadeimplantadora' ? 'checked="checked"' : ''; ?>/>&nbsp;Unidade implantadora&nbsp;
				<input type="radio" name="tipo" value="tipoobra" <?php echo $tipo == 'tipoobra' ? 'checked="checked"' : ''; ?>/>&nbsp;Tipo de obra&nbsp;
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td>
				<input type="button" value="Pesquisar" id="btn_pesquisar" />
				<input type="button" value="Limpar" id="btn_limpar" />
			</td>
		</tr>
	</table>
</form>
<?php 

if($_POST){
	
	switch ($_POST['tipo']){
		case 'fonte':
			$sql = "select tooid, toodescricao from obras.tipoorigemobra  ".($_POST['descricao'] ? " where (toodescricao ilike '%".$_POST['descricao']."%' or removeacento(toodescricao) ilike '%".$_POST['descricao']."%' )" : "")." order by toodescricao";
			break;
		case 'tipologia':
			$sql = "select tpoid, tpodsc from obras.tipologiaobra  where orgid = 3 ".($_POST['descricao'] ? " and (tpodsc ilike '%".$_POST['descricao']."%' or removeacento(tpodsc) ilike '%".$_POST['descricao']."%' )" : "")." order by tpodsc";
			break;
		case 'programa':
			$sql = "select prfid, prfdesc from obras.programafonte  where orgid = 3 ".($_POST['descricao'] ? " and (prfdesc ilike '%".$_POST['descricao']."%' or removeacento(prfdesc) ilike '%".$_POST['descricao']."%' )" : "")." order by prfdesc";
			break;
		case 'unidadeimplantadora':
			$sql = "select distinct ent.entid, ent.entnumcpfcnpj || ' - ' || ent.entnome || ' - ' || mun.mundescricao || '/' || mun.estuf as entnome from entidade.entidade ent
					inner join entidade.funcaoentidade fun on ent.entid = fun.entid and fuestatus = 'A'
					left join entidade.endereco ede on ede.entid = ent.entid and endstatus = 'A'
					left join territorios.municipio mun on mun.muncod = ede.muncod   
					where fun.funid in (1,6)
					and entstatus = 'A'
					".($_POST['descricao'] ? " and (entnome ilike '%".$_POST['descricao']."%' or removeacento(entnome) ilike '%".$_POST['descricao']."%' )" : "")."";
			break;
		case 'tipoobra':
			$sql = "select tobaid, tobadesc from obras.tipoobra  ".($_POST['descricao'] ? " where (tobadesc ilike '%".$_POST['descricao']."%' or removeacento(tobadesc) ilike '%".$_POST['descricao']."%' )" : "")." order by tobadesc";
			break;
	}
	
	if(isset($sql)){	
		$cabecalho = array('Código', 'Descrição');
		$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', array(60));
	}
}

?>