<?php
$arrPerfil = pegaPerfilGeral();
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
		/*
		$sql = "select c.curid as codigo, c.curdesc || '( ' || COALESCE(m.moddesc,'-') || ' )' as descricao from catalogocurso.curso c
				left join catalogocurso.modalidadecurso m using(modid) 
				where c.curstatus = 'A' and c.ateid = {$_REQUEST['ateid']} order by c.curdesc";
		*/
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
		$db->monta_combo('curid', $sql, 'S', 'Selecione...', '', '', '', '', 'N','curid');
	}else{
		echo "Selecione a Área Temática.";	 
	}
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";
$boAtivo = 'S';
monta_titulo('Visualização da Demanda',  obrigatorio()." Indica campos obrigatórios.");


// limpando sessão que armazena os filtros
if($_REQUEST['limparfiltros']) {
	unset($_SESSION['snf_var']);
}

// gravando filtros para mante-los apos navegação
if($_POST['estuf']) {
	$_SESSION['snf_var'] = $_POST; 
}


if($_POST){
	extract($_POST);
} elseif($_SESSION['snf_var']) {
	extract($_SESSION['snf_var']);
	$_POST = $_SESSION['snf_var'];
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
function abreDetalheCurso(curid,ateid,modid,ncuid,pcfano)
{
	var url = "snf.php?modulo=principal/detalhesCurso&acao=A&curid=" + curid + "&ateid=" + ateid + "&modid=" + modid  + "&ncuid=" + ncuid + "&pfcid=" + pcfano;
	janela(url,1024,768,'Curso')
}
function ofertaCurso(curid,modid,ncuid)
{
	window.location.href="snf.php?modulo=principal/ofertaCurso&acao=A&curid=" + curid + "&modid=" + modid + "&ncuid=" + ncuid;
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
				$sql = "select ateid as codigo,atedesc as descricao from catalogocurso.areatematica where atestatus = 'A' order by atedesc";
				$db->monta_combo('ateid', $sql, $boAtivo, 'Selecione...', 'carregaCursos', '', '', '', 'N','ateid');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Curso</td>
			<td id="td_combo_cursos" >
				<?php
				if($ateid){	
					/*
					$sql = "select curid as codigo, curdesc || '( ' || COALESCE(m.moddesc,'-') || ' )' as descricao from catalogocurso.curso c 
					left join catalogocurso.modalidadecurso m using(modid)
					where curstatus = 'A' and ateid = $ateid order by curdesc";
					*/
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
		<tr>
			<td class="subtituloDireita">Período</td>
			<td>
				<?php
				//$sql = "select pcfid as codigo, pcfano as descricao from pdeinterativo.periodocursoformacao where pcfstatus = 'A' ";
				$arrAno[0] = array("codigo" => "2012","descricao" => "2012");
				$arrAno[1] = array("codigo" => "2013","descricao" => "2013");
				$arrAno[2] = array("codigo" => "2014","descricao" => "2014");
				$arrAno[3] = array("codigo" => "2015","descricao" => "2015");
				$arrAno[4] = array("codigo" => "2016","descricao" => "2016");
				$pcfid = !$pcfid ? date('Y') : $pcfid;
				$db->monta_combo('pcfid', $arrAno, $boAtivo, 'Selecione...', '', '', '', '', 'N','pcfid');
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
				<input type="button" value="Limpar" id="btn_limpar" onclick="window.location.href='snf.php?modulo=principal/visualizarDemanda&acao=A&limparfiltros=1'" />
			</td>
		</tr>
	</table>
</form>
<?php 
if($_POST['curid']){
	
	$_POST['modid'] = substr($_POST['curid'], -1);
	$_POST['curid'] = substr($_POST['curid'], 0, -1);
	
	$arrWhere[]  = "p.curid = '{$_POST['curid']}'";
	$arrWhere2[] = "p.curid = '{$_POST['curid']}'";
}
if($_POST['ncuid']){
	$arrWhere[]  = "p.ncuid = '{$_POST['ncuid']}'";
	$arrWhere2[] = "c.ncuid = '{$_POST['ncuid']}'";
}
if($_POST['modid']){
	$arrWhere[]  = "p.modid = '{$_POST['modid']}'";
	$arrWhere2[] = "m.modid = '{$_POST['modid']}'";
}
if($_POST['ateid']){
	$arrWhere[]  = "p.ateid = '{$_POST['ateid']}'";
	$arrWhere2[] = "a.ateid = '{$_POST['ateid']}'";
}
if($_POST['estuf']){
	$arrWhere[]  = "p.estuf = '{$_POST['estuf']}'";
	$arrWhere2[] = "p.estuf = '{$_POST['estuf']}'";
}
if($_POST['muncod']){
	$arrWhere[]  = "p.muncod = '{$_POST['muncod']}'";
}

if($pcfid){
	$arrWhere[]  = "p.pcfano in ($pcfid)";
	$arrWhere2[] = "p.demanoreferencia in ($pcfid)";
}

if(!in_array(PERFIL_CONSULTA,$arrPerfil) || count($arrPerfil) > 1 ){
	$acao  = "'<img onclick=\"abreDetalheCurso(' || p.curid || ',' || p.ateid || ',' || p.modid || ',' || p.ncuid || ',\'' || p.pcfano || '\')\" onmouseover=\"return escape(\'Detalhar Curso\');\" src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" /> <img src=\"../imagens/editar_nome.gif\" onmouseover=\"return escape(\'Realizar pré-oferta\');\" onclick=\"ofertaCurso(' || p.curid || ',' || p.modid || ',' || p.ncuid || ')\" style=\"cursor:pointer\" />' as acao,";
	$acao2 = "'<img onclick=\"abreDetalheCurso(' || p.curid || ',' || a.ateid || ',' || m.modid || ',' || c.ncuid || ',\'' || p.demanoreferencia || '\')\" onmouseover=\"return escape(\'Detalhar Curso\');\" src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" />
			   &nbsp;
			   <img src=\"../imagens/editar_nome.gif\" onmouseover=\"return escape(\'Realizar pré-oferta\');\" onclick=\"ofertaCurso(' || p.curid || ',' || c.modid || ',' || c.ncuid || ')\" style=\"cursor:pointer\" />' as acao,";
}else{
	$acao  = "'<img onclick=\"abreDetalheCurso(' || p.curid || ',' || p.ateid || ',' || p.modid || ',' || p.ncuid || ',\'' || p.pcfano || '\')\" onmouseover=\"return escape(\'Detalhar Curso\');\" src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" />' as acao,";
	$acao2 = "'<img onclick=\"abreDetalheCurso(' || p.curid || ',' || a.ateid || ',' || m.modid || ',' || c.ncuid || ',\'' || p.demanoreferencia || '\')\" onmouseover=\"return escape(\'Detalhar Curso\');\" src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" />' as acao,";
}

$sql = "select * from (
			select distinct
			$acao
			p.estuf,
			m.mundescricao,
			'<span style=\"display:none\" >' || upper(p.curdesc) || '</span><span style=\"cursor:pointer\" onclick=\"abreDetalheCurso(' || p.curid || ',' || p.ateid || ',' || p.modid || ',' || p.ncuid || ',\'' || p.pcfano || '\')\" >' || upper(p.curdesc) || '</span>' as curso,
			p.moddesc,
			p.ncudesc,
			'<div style=\"color:#0066CC;text-align:right\" >' || p.pcfano || '</div>' as ano,
            coalesce(sum(p.privagasprevistas),0) +  coalesce(sum(p.pridemvagasprevistas),0) as demanda
		from snf.prioridadecursoescola p
        inner join territorios.municipio m using(muncod) 
        inner join snf.prioridadedocumento pd using(prdid)
        inner join workflow.documento d using(docid)
		where p.pristatus = 'A'
	    	and d.esdid = 464
			".($arrWhere ? " and ".implode(" and ", $arrWhere) : "")."
		group by p.curid, p.estuf, m.muncod,  m.mundescricao, 
            p.curid, p.curdesc, p.ncuid, p.ncudesc, 
        	p.modid, p.moddesc, p.pcfano,p.ateid 
        	
        	union all (
        	
        	
		select 
        	$acao2 
            p.estuf, 
            'MEC', 
            '<span style=\"display:none\" >' || upper(c.curdesc) || '</span><span style=\"cursor:pointer\" onclick=\"abreDetalheCurso('|| p.curid || ',' || a.ateid || ',' || m.modid || ',' || n.ncuid || ',\'' || p.demanoreferencia || '\')\">' || upper(c.curdesc) || '<\span>' as curso,
            m.moddesc, 
            n.ncudesc, 
            '<div style=\"color:#0066CC;text-align:right\" >' || p.demanoreferencia  || '</div>' as ano,
            p. demvagassolicitadas as demanda
        from snf.demandainterna p
        inner join catalogocurso.curso c using(curid)
        inner join catalogocurso.modalidadecurso m using(modid)
        inner join catalogocurso.nivelcurso n using(ncuid)
        inner join catalogocurso.areatematica a  on c.ateid = a.ateid 
        ".($arrWhere2 ? " where ".implode(" and ", $arrWhere2) : "")."
       	
       	)
		) as foo where demanda > 0";
                   
                   
$cabecalho = array('Ação','UF','Município','Curso','Modalidade','Nível','Período','Demanda');


if($_POST['estuf']){
	$db->monta_lista($sql, $cabecalho, 100, 10, 'S', '', '', '', '');
}else{
	echo "<center><div style='color:#990000;font-weight:bold;margin-top:3px' >Selecione a UF para exibir os cursos.</div></center>";;
}
?>