<?php
if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$sql = "select 
				sis.sisdiretorio
			from 
				public.arquivo arq
			inner join
				seguranca.sistema sis ON sis.sisid = arq.sisid
			where 
					arqid = {$_GET['arquivo']}";
	$schema = $db->pegaUm($sql);
	$file = new FilesSimec(null,null,$schema);
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';  
echo "<br>";
monta_titulo('Relatório de Adesão',''); 

$sql = "select
			count(distinct uni.uniid) as qtde_instituicoes,
			--count(distinct preadesao.uniid) as qtde_preadesao,
			count(distinct adesao.uniid) as qtde_adesao--,
			--count(distinct adesao_preadesao.uniid) as qtde_adesao_preadesao
		FROM
			maismedicos.universidade uni
		left join
			maismedicos.maismedicos preadesao ON uni.uniid = preadesao.uniid and preadesao.msminteresse is true
		left join
			maismedicos.maismedicos adesao ON uni.uniid = adesao.uniid and adesao.msmadesao is true
		left join
			maismedicos.maismedicos adesao_preadesao ON uni.uniid = adesao_preadesao.uniid and adesao_preadesao.msminteresse is true and adesao_preadesao.msmadesao is true
		where
			uni.unistatus = 'A'";
$arrTotal = $db->pegaLinha($sql);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="25%" class="subtituloDireita" >Total de Instituições Supervisoras</td>
		<td>
			<a href="javascript:filtrarTodos()" ><?php echo number_format($arrTotal['qtde_instituicoes'],0,',','.')?></a>
		</td>
	</tr>
	<!-- 
	<tr>
		<td class="subtituloDireita" >Total de Pré-Adesão</td>
		<td>
			<a href="javascript:filtrarPreAdesao()" ><?php echo number_format($arrTotal['qtde_preadesao'],0,',','.')?></a>
		</td>
	</tr>
	 -->
	<tr>
		<td class="subtituloDireita" >Total de Adesão</td>
		<td>
			<a href="javascript:filtrarAdesao()" ><?php echo number_format($arrTotal['qtde_adesao'],0,',','.');?></a>
		</td>
	</tr>
	<!-- 
	<tr>
		<td class="subtituloDireita" >Total de Pré-Adesão e Adesão</td>
		<td>
			<a href="javascript:filtrarAdesaoPreAdesao()" ><?php echo number_format($arrTotal['qtde_adesao_preadesao'],0,',','.');?></a>
		</td>
	</tr>
	 -->
</table>
<form name="pesquisa" id="pesquisa" method="post">
<input type="hidden" id="filtro" name="filtro" value="filtro" />
</form>
<?php
if($_POST['filtro'] == "preadesao"){
	$arrWhere[] = "msm.msminteresse is true";
}
if($_POST['filtro'] == "adesao"){
	$arrWhere[] = "msm.msmadesao is true";
}
if($_POST['filtro'] == "adesao_preadesao"){
	$arrWhere[] = "msm.msminteresse is true";
	$arrWhere[] = "msm.msmadesao is true";
}

$sql = "SELECT
					CASE WHEN unisigla <> '' THEN
						'<a style=\"cursor:pointer;\" onclick=\"abreDadosPagamento( ' || uni.uniid || ' );\">' || UPPER(uni.unisigla) ||  ' - ' || UPPER(uni.uninome) || '</a>'
						ELSE
						'<a style=\"cursor:pointer;\" onclick=\"abreDadosPagamento( ' || uni.uniid || ' );\">' || UPPER(uni.uninome) ||  '</a>' END as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					/*case when msminteresse is true
						then '<center><img src=\"../imagens/check.jpg\"  /></center>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as preadesao,
					case when msm.arqid is not null 
						then '<a href=\"javascript:downloadArquivo(' || msm.arqid || ')\" >' || arq.arqnome || '.' || arq.arqextensao || '</a>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as anexo,*/
					case when msmadesao is true
						then '<center><img src=\"../imagens/check.jpg\"  /></center>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as adesao,
					case when msm.arqidtermo is not null 
						then '<a href=\"javascript:downloadArquivo(' || msm.arqidtermo || ')\" >Termo de Adesão</a>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as anexo2
				FROM
					maismedicos.universidade uni 
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = uni.muncod
				LEFT JOIN
					maismedicos.maismedicos msm ON uni.uniid = msm.uniid
				LEFT JOIN
					public.arquivo arq ON arq.arqid = msm.arqid
				LEFT JOIN
					public.arquivo arq2 ON arq2.arqid = msm.arqidtermo
				WHERE
					uni.unistatus = 'A'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				--GROUP BY uni.uniid, uni.uninome , uni.unisigla, mun.mundescricao, mun.estuf, msm.msminteresse, msm.msmsetorresponsavel, msm.arqid, arq.arqnome, arq.arqextensao
				ORDER BY
					 uni.unisigla, uni.uninome";
$cabecalho = array('Instituiçãos Supervisora', 'Município', 'UF','Adesão','Termo de Adesão');
//dbg($sql,1);
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');
?>
<script>
function downloadArquivo(arqid)
{
	window.location.href="maismedicos.php?modulo=relatorio/preAdesao&acao=C&arquivo="+arqid;
}
function abreDadosPagamento( uniid ){
	window.location.href = 'maismedicos.php?modulo=principal/maisMedicos&acao=A&uniid=' + uniid;
}
function filtrarPreAdesao()
{
	document.getElementById('filtro').value = 'preadesao';
	document.getElementById('pesquisa').submit();
}
function filtrarAdesao()
{
	document.getElementById('filtro').value = 'adesao';
	document.getElementById('pesquisa').submit();
}
function filtrarTodos()
{
	document.getElementById('filtro').value = '';
	document.getElementById('pesquisa').submit();
}
function filtrarAdesaoPreAdesao()
{
	document.getElementById('filtro').value = 'adesao_preadesao';
	document.getElementById('pesquisa').submit();
}
</script>