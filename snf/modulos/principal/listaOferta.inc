<?php
unset($_SESSION['snf']);
if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	exit;
}
function carregaMunicipios()
{
	global $db;
	if($_REQUEST['estuf']){
		$sql = "select 
					muncod as codigo,
					mundescricao as descricao
				from 
					territorios.municipio
				where
					estuf = '{$_REQUEST['estuf']}'
				order by
					mundescricao";
		
		$db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '', 'N','muncod');
	}else{
		echo "Selecione a UF.";
	}
}
function carregaCursos()
{
	global $db;
	if($_REQUEST['ateid']){	
		//$sql = "select curid as codigo, curdesc as descricao from catalogocurso.curso where curstatus = 'A' and ateid = {$_REQUEST['ateid']}";
		$ateid = $_REQUEST['ateid'];
		$sql = "select c.curid ||''|| mc.modid as codigo, c.curdesc || '(' || n.ncudesc || ' - '  || m.moddesc || ')' as descricao
							from catalogocurso.curso c
							inner join catalogocurso.modalidadecurso_curso mc
								on mc.curid = c.curid
							inner join catalogocurso.modalidadecurso m
								on mc.modid = m.modid
							natural join catalogocurso.nivelcurso n
							where c.curstatus = 'A'
							and c.ateid = $ateid
							order by 2";
		$db->monta_combo('curid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N','curid');
	}else{
		echo "Selecione a Área Temática.";	 
	}
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";
$boAtivo = 'S';
monta_titulo('Lista de Ofertas',  obrigatorio()." Indica campos obrigatórios.");
if($_POST){
	extract($_POST);
}
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
function carregaMunicipios(estuf)
{
	$.ajax({
		url		: 'snf.php?modulo=principal/visualizarDemanda&acao=A',
		type	: 'post',
		data	: 'requisicaoAjax=carregaMunicipios&estuf='+estuf,
		success	: function(res){
			$('#td_combo_municipio').html(res);
		}
	});	
}
function carregaCursos(ateid)
{
	$.ajax({
		url		: 'snf.php?modulo=principal/visualizarDemanda&acao=A',
		type	: 'post',
		data	: 'requisicaoAjax=carregaCursos&ateid='+ateid,
		success	: function(res){
			$('#td_combo_cursos').html(res);
		}
	});	
}
function pesquisarDemanda()
{
	$('#formulario_id').submit();
}
function ofertaCurso(curid)
{
	window.location.href="snf.php?modulo=principal/ofertaCurso&acao=A&curid=" + curid;
}
function exibeMapa(curid,ateid,modid,ncuid)
{
	var url = "snf.php?modulo=principal/detalhesCurso&acao=A&curid=" + curid + "&ateid=" + ateid + "&modid=" + modid  + "&ncuid=" + ncuid + "&aba=mapa";
	janela(url,1024,768,'Curso')
}

</script>
<form name="formulario" id="formulario_id" method="post" action="">
	<input type="hidden" 
	name="requisicao" id="requisicao" value="" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td width="25%" class="subtituloDireita">UF</td>
			<td>
				<?php
				$sql = "select
							estuf as codigo,
							estdescricao as descricao
						from 
							territorios.estado
						order by
							estdescricao";
				$db->monta_combo('estuf', $sql, $boAtivo, 'Selecione...', 'carregaMunicipios', '', '', '', 'S','estuf');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td id="td_combo_municipio">
				<?php
				if($estuf){	
					$sql = "select
								muncod as codigo,
								mundescricao as descricao
							from
								territorios.municipio
							where
								estuf = '$estuf'";
					$db->monta_combo('muncod', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N','muncod');
				}else{
					echo "Selecione a UF.";	 
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Área Temática</td>
			<td>
				<?php
				$sql = "select ateid as codigo,atedesc as descricao from catalogocurso.areatematica where atestatus = 'A'";
				$db->monta_combo('ateid', $sql, $boAtivo, 'Selecione...', 'carregaCursos', '', '', '', 'N','ateid');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Curso</td>
			<td id="td_combo_cursos" >
				<?php
				if($ateid){	
					//$sql = "select curid as codigo, curdesc as descricao from catalogocurso.curso where curstatus = 'A' and ateid = $ateid";
					$sql = "select c.curid ||''|| mc.modid as codigo, c.curdesc || '(' || n.ncudesc || ' - '  || m.moddesc || ')' as descricao
							from catalogocurso.curso c
							inner join catalogocurso.modalidadecurso_curso mc
								on mc.curid = c.curid
							inner join catalogocurso.modalidadecurso m
								on mc.modid = m.modid
							natural join catalogocurso.nivelcurso n
							where c.curstatus = 'A'
							and c.ateid = $ateid
							order by 2";
					$db->monta_combo('curid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N','curid','',$curid.$modid);
				}else{
					echo "Selecione a Área Temática.";	 
				}
				?>
			</td>
		</tr>
		<!-- 
		<tr>
			<td class="subtituloDireita">Modalidade</td>
			<td>
				<?php
				//$sql = "select modid as codigo, moddesc as descricao from catalogocurso.modalidadecurso where modstatus = 'A'";
				//$db->monta_combo('modid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N','modid');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nível</td>
			<td>
				<?php
				//$sql = "select ncuid as codigo, ncudesc as descricao from catalogocurso.nivelcurso where ncustatus = 'A'";
				//$db->monta_combo('ncuid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N','ncuid');			 
				?>
			</td>
		</tr>
		 -->
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Buscar" id="btn_salvar" onclick="pesquisarDemanda()" />				
				<input type="button" value="Limpar" id="btn_limpar" onclick="window.location.href='snf.php?modulo=principal/listaOferta&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<?php 
if($_POST['curid']){
	
	$_POST['modid'] = substr($_POST['curid'], -1);
	$_POST['curid'] = substr($_POST['curid'], 0, -1);
	
	$arrWhere[] = "curid = '{$_POST['curid']}'";
}
if($_POST['ncuid']){
	$arrWhere[] = "ncuid = '{$_POST['ncuid']}'";
	$_SESSION['snf']['ncuid'] = $_POST['ncuid'];
}
if($_POST['modid']){
	$arrWhere[] = "modid = '{$_POST['modid']}'";
	$_SESSION['snf']['modid'] = $_POST['modid'];
}
if($_POST['ateid']){
	$arrWhere[] = "ateid = '{$_POST['ateid']}'";
	$_SESSION['snf']['ateid'] = $_POST['ateid'];
}
if($_POST['estuf']){
	$arrWhere[] = "estuf = '{$_POST['estuf']}'";
}
if($_POST['muncod']){
	$arrWhere[] = "muncod = '{$_POST['muncod']}'";
}

$sql = "select '<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"exibeMapa(' || curid || ',' || ateid || ',' || modid || ',' || ncuid || ')\"  />' as acao,estuf,mundescricao,atedesc,curdesc,moddesc,ncudesc,tipo,nome,ofeqtde from (
select o.curid, cu.curdesc,  a.ateid, a.atedesc, o.ofeid, mo.modid, mo.moddesc, nc.ncuid, ncudesc, en.estuf, m.muncod, m.mundescricao, 'Pólo UAB' as tipo, pu.pobdescricao as nome, o.ofeqtde
   from snf.oferta o
            inner join snf.polouab pu
            using(pobid)
            inner join snf.instituicaouab iu
            on iu.pobid = pu.pobid
            inner join snf.instituicaoensino ies
            on ies.insid = iu.insid
            inner join snf.enderecopolo en
            on en.endid = pu.endid
            inner join territorios.municipio m
            on m.muncod = en.muncod
	    inner join catalogocurso.curso cu
	    using(curid)
	    inner join catalogocurso.areatematica a
	    using(ateid)
	    inner join catalogocurso.modalidadecurso mo
	    on mo.modid = o.modid 
	    inner join catalogocurso.nivelcurso nc
	    using(ncuid)
UNION
	select o.curid, cu.curdesc,  a.ateid, a.atedesc, o.ofeid, mo.modid, mo.moddesc, nc.ncuid, ncudesc, en.estuf, m.muncod, m.mundescricao, 'Pólo' as tipo,pu.poldescricao as nome, o.ofeqtde
            from snf.oferta o
            inner join snf.polo pu
            using(polid)
            inner join snf.instituicaoensino ies
            on ies.insid = pu.insid
            inner join snf.enderecopolo en
            on en.endid = pu.endid
            inner join territorios.municipio m
            on m.muncod = en.muncod
	    inner join catalogocurso.curso cu
	    using(curid)
	    inner join catalogocurso.areatematica a
	    using(ateid)
	    inner join catalogocurso.modalidadecurso mo
	    on mo.modid = o.modid 
	    inner join catalogocurso.nivelcurso nc
	    using(ncuid)

            UNION 
            select o.curid, cu.curdesc, a.ateid, a.atedesc, o.ofeid, mo.modid, mo.moddesc, nc.ncuid, ncudesc, en.estuf, m.muncod, m.mundescricao, 'Campus' as tipo,ent.entnome as nome, o.ofeqtde
            from snf.oferta o
            inner join academico.campus c
            on o.cmpid = c.cmpid 
            inner join entidade.entidade ent
            on c.entid = ent.entid
            inner join entidade.funcaoentidade fen
            on fen.entid = ent.entid
            and fen.funid = 18
            inner join entidade.funentassoc fea
            on fea.fueid = fen.fueid
            inner join entidade.entidade I
            on I.entid = fea.entid
            inner join entidade.funcaoentidade fies
            on fies.entid = I.entid
            and fies.funid in (11,12)
            inner join snf.instituicaoensino ies
            on ies.entid = fies.entid
            inner join entidade.endereco en
            on c.entid = en.entid
            inner join territorios.municipio m
            on m.muncod = en.muncod
	    inner join catalogocurso.curso cu
	    using(curid)
	    inner join catalogocurso.areatematica a
	    using(ateid)
	    inner join catalogocurso.modalidadecurso mo
	    on mo.modid = o.modid 
	    inner join catalogocurso.nivelcurso nc
	    using(ncuid)
		) as oferta where 1=1 ".($arrWhere ? " and ".implode(" and ", $arrWhere) : "");
$cabecalho = array('Ação','UF','Município','Área Temática','Curso','Modalidade','Nível','Tipo de Unidade de Oferta','Unidade de Oferta','Oferta');
if($_POST['estuf']){
	$db->monta_lista($sql, $cabecalho, 100, 10, 'N', '', '', '', '');
}else{
	echo "<center><div style='color:#990000;font-weight:bold;margin-top:3px' >Selecione a UF para exibir os cursos.</div></center>";;
}
?>