<?php

if($_GET['unicod']){
	// verificando a unidade
	$unidade = $db->pegaLinha("SELECT * FROM public.unidade WHERE unicod='".$_GET['unicod']."'");
} else {
	die("<script>alert('Unidade não encontrada');window.close();</script>");
}

// verificando se é undidade ou unidade gestora
/* -- DESATIVADO
if ($_REQUEST['sbaid']){
    $sql = "SELECT count(1) as contador FROM monitora.pi_subacao a inner join monitora.pi_subacaounidade b ON b.sbaid = a.sbaid WHERE a.sbaid ={$_REQUEST['sbaid']} and b.unicod = '".$_GET['unicod']."'";
	$verificaSubacaoUnidade = $db->pegaUm($sql);
        ver($sql);
}

# Filtro para a unidade indireta 
if ($verificaSubacaoUnidade) {
$where .= $_REQUEST['sbaid'] ? "AND sad.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula1 .= $_REQUEST['sbaid'] ? "where sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula2 .= $_REQUEST['sbaid'] ? "AND sbaid = '".$_REQUEST['sbaid']."'" : '';
} else {
$clausula1 .= 'where 1 = 2';
}

/* Filtro para a unidade direta 
if ($_REQUEST['sbaid'] && $verificaSubacaoUnidade){
$where_direta .= $_REQUEST['sbaid'] ? "AND sad.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula1_direta .= $_REQUEST['sbaid'] ? "and a.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula2_direta .= $_REQUEST['sbaid'] ? "AND sbaid = '".$_REQUEST['sbaid']."'" : '';	
}
/* Filtro para a unidade indireta 
$where .= $_POST['prgcod'] ? "AND UPPER(aca.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where .= $_REQUEST['unicod'] ? "AND uni.unicod = '{$_REQUEST['unicod']}'" : '';
$where .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

/* Filtro para a unidade direta 	
$where_direta .= $_POST['prgcod'] ? "AND UPPER(aca.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where_direta .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where_direta .= $_REQUEST['unicod'] ? "AND uni.unicod = '{$_REQUEST['unicod']}'" : '';
$where_direta .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

$unidsc    = $unidade['entunicod']." - ".$unidade['unidsc'];
$unicod    = $unidade['entunicod'];

if($_SESSION['obra']['orgid'] == 1 && possuiPerfil(array(PERFIL_EMPRESA,PERFIL_SUPERVISORMEC,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC))){
	$st_where = " or aca.unicod = '26101' ";
}

if(possuiPerfil(array(PERFIL_EMPRESA,PERFIL_SUPERVISORMEC,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC))){

    $sql_lista .= <<<DML
        SELECT
            '<input type=\"checkbox\" id=\"chk_'||ptr.ptres||'\" onclick=\"resultado(this,\''||ptr.ptrid||'\',\''||ptr.ptres||'\');\">' as checkbox, 
            ptr.ptres, 
            trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) as descricao, 
            uni.unicod || ' - ' || uni.unidsc as unidsc, 
            ptr.ptrdotacao as dotacaoatual, 
            coalesce(dt.valor,0.00) as det_subacao, 
            coalesce(ptr.ptrdotacao,0.00) - coalesce(dt.valor,0.00) as nao_det_subacao,
            coalesce(sum(pip.valorpi),0.00) as det_pi, 
            coalesce(ptr.ptrdotacao,0.00) - coalesce(sum(pip.valorpi),0.00) as nao_det_pi,
            coalesce(pemp.total,0.00) as empenhado,
            coalesce(sum(ptr.ptrdotacao),0.00) - coalesce (pemp.total,0.00) as nao_empenhado            
        FROM monitora.acao aca
        INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
        INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
        LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid 
        LEFT JOIN ( 
            SELECT a.sbaid, a.ptrid, sum(sadvalor) as valor
            FROM monitora.pi_subacaodotacao a
            INNER JOIN monitora.pi_subacao b ON b.sbaid = a.sbaid
            WHERE b.pieid IS NOT NULL
                {$clausula1_direta}
            GROUP BY a.sbaid, a.ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
        LEFT JOIN ( 
            SELECT pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
            FROM monitora.pi_planointerno pi 
            INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
            INNER JOIN monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
            WHERE pi.plistatus = 'A' 
                {$clausula2_direta}
            GROUP BY pi.sbaid, ptres.ptrid, ptrano) pip ON ptr.ptrid = pip.ptrid 
        LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = '{$_GET['anoexercicio']}' AND pemp.unicod = ptr.unicod)
        WHERE aca.prgano = '{$_GET['anoexercicio']}'
            AND aca.acasnrap = false 
DML;
						  
    $sql_lista .= $where_direta ? $where_direta : '';
    $sql_lista .= " GROUP BY ptr.ptrid, ptr.ptres,ptr.ptrdotacao, trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ,
        uni.unidsc, dt.valor, uni.unicod, pemp.total";
} else {
					
    $sql_lista = <<<DML
        SELECT
            '<input type=\"checkbox\" id=\"chk_'||ptr.ptres||'\" onclick=\"resultado(this,\''||ptr.ptrid||'\',\''||ptr.ptres||'\');\">' as checkbox, 
            ptr.ptres, 
            trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) as descricao, 
            uni.unicod || ' - ' || uni.unidsc as unidsc, 
            ptr.ptrdotacao as dotacaoatual, 
            coalesce(dt.valor,0.00) as det_subacao, 
            coalesce(ptr.ptrdotacao,0.00) - coalesce(dt.valor,0.00) as nao_det_subacao,
            coalesce(sum(pip.valorpi),0.00) as det_pi, 
            coalesce(ptr.ptrdotacao,0.00) - coalesce(sum(pip.valorpi),0.00) as nao_det_pi,
            coalesce(pemp.total,0.00) as empenhado,
            coalesce(sum(ptr.ptrdotacao),0.00) - coalesce (pemp.total,0.00) as nao_empenhado
        FROM monitora.acao aca
        INNER JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
        INNER JOIN public.unidade uni ON uni.unicod = ptr.unicod
        LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid 
        LEFT JOIN ( 
            SELECT sbaid, ptrid, sum( sadvalor ) as valor
            FROM monitora.pi_subacaodotacao
                {$clausula1}
            GROUP BY sbaid, ptrid ) dt ON ptr.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
        LEFT JOIN ( 
            SELECT pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
            FROM monitora.pi_planointerno pi 
            INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
            INNER JOIN monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
            WHERE pi.plistatus = 'A' 
                {$clausula2}
            GROUP BY pi.sbaid, ptres.ptrid, ptrano) pip ON ptr.ptrid = pip.ptrid 
        LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = '{$_GET['anoexercicio']}' AND pemp.unicod = ptr.unicod)
        WHERE aca.prgano = '{$_GET['anoexercicio']}'
            AND (aca.unicod = '{$_GET['unicod']}')
            AND aca.acasnrap = false 
DML;

    $sql_lista .= $where ? $where : ''; 
    $sql_lista .= "GROUP BY ptr.ptrid, ptr.ptres,ptr.ptrdotacao, trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ,
        uni.unidsc,uni.unicod,pemp.total ";    
}*/
/* Filtro para a UO */
$perfis = pegaPerfilGeral();
if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)

DML;
    $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}

// verificando se é undidade ou unidade gestora
$where .= $_REQUEST['sbaid'] && $_REQUEST['sbaid'] != 'null' ? " AND ptr.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = " . $_REQUEST['sbaid'] . ")" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(ptr.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%" . $_POST['buscalivre'] . "%') OR ptr.ptres ilike '%" . $_POST['buscalivre'] . "%')" : '';

/* Modifica a conta para o limite de detalhamento */
$detalhamentoPi = "(COALESCE(ptr.ptrdotacao, 0.00) - COALESCE(SUM(pip.pipvalor), 0.00)) AS nao_det_pi,";
if($_REQUEST['sbaid'] && $_REQUEST['sbaid'] != 'null'){
    $detalhamentoPi = "(COALESCE(SUM(dts.valor), 0.00) - COALESCE(SUM(pip.pipvalor), 0.00)) AS nao_det_pi,";
}
$sql_lista = <<<DML
    SELECT 
        '<input type=\"checkbox\" id=\"chk_'||ptr.ptres||'\" onclick=\"resultado(this,\''||ptr.ptrid||'\',\''||ptr.ptres||'\');\">' as checkbox, 
        ptr.ptres,
        trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
        aca.unicod || ' - ' || uni.unidsc as unidsc,
        COALESCE(ptr.ptrdotacao, 0.00) AS dotacaoatual,
        COALESCE(SUM(dts.valor), 0.00) AS det_subacao,
        (COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(SUM(dts.valor), 0.00)) AS nao_det_subacao,
        COALESCE(SUM(pip.pipvalor), 0.00) AS det_pi,
        $detalhamentoPi
        COALESCE((pemp.total), 0.00) AS empenhado,
        COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
    FROM monitora.ptres ptr
    INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid
    INNER JOIN public.unidade uni on aca.unicod = uni.unicod
    LEFT JOIN (SELECT pip2.ptrid, pipvalor  FROM monitora.pi_planointernoptres pip2 JOIN monitora.pi_planointerno USING (pliid) WHERE plistatus ='A') pip on ptr.ptrid = pip.ptrid
    LEFT JOIN (SELECT ptrid, SUM(sadvalor) AS valor 
        FROM monitora.pi_subacaodotacao
        GROUP BY ptrid) dts ON dts.ptrid = ptr.ptrid
    LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = '{$_SESSION['exercicio']}' AND pemp.unicod = ptr.unicod)
    WHERE ptr.ptrano = '{$_SESSION['exercicio']}'
        
        $where
        $whereUO
        AND ptr.ptrstatus = 'A'
    GROUP BY ptr.ptrid, ptr.ptres,aca.prgcod, aca.acacod,aca.unicod,aca.loccod,aca.acatitulo,uni.unidsc, ptr.ptrdotacao, pemp.total
    ORDER BY ptr.ptres
DML;
//ver($_SESSION['exercicio']);
?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php monta_titulo($entnome, '&nbsp'); ?>
<form action="" method="post" name="formulario">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Programa:</td>
	<td>
	<?php
        $prgcod = $_REQUEST['prgcod'];
        $sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
                FROM monitora.programa p
                WHERE p.prgano = '".$_GET['anoexercicio']."' ORDER BY prgcod";
        $db->monta_combo('prgcod', $sql, 'S','Selecione','','','',400);
	?>
	</td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Ação:</td>
	<td>
	<?php
        $sql = "
            SELECT
                ac.acacod as codigo,
                (ac.acacod || ' - ' || ac.acadsc) as descricao
            FROM monitora.acao ac 
            WHERE
                ac.unicod = '".$_GET['unicod']."'
                AND ac.prgano = '".$_GET['anoexercicio']."'
                AND ac.acastatus = 'A'
                AND ac.acasnrap = FALSE
            GROUP BY
                ac.acacod,
                ac.acadsc
            ORDER BY
                ac.acacod";
        $acacod = $_REQUEST['acacod'];
        $db->monta_combo('acacod', $sql, 'S', 'Selecione','','','',400);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Buscar:</td>
	<td>
	<?php
        $buscalivre = $_REQUEST['buscalivre'];
        echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, '' );
    ?>
	</td>
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<input type="submit" name="botao" value="Pesquisar"/>
	<input type="button" name="botao" value="Todos" onclick="window.location='?modulo=principal/planotrabalho/listarPrograma&acao=A';"/>
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
<br>
<?php
$cabecalho = array ("","PTRES",
                "Ação",
                "Unidade Orçamentária",
                "Dotação&nbsp;Atual&nbsp;(R$)",
                'Detalhado Subação',
                "Não Detalhado Subação",
                "Detalhado PI",
                "Não Detalhado PI",
                'Empenhado',
                'Não Empenhado');
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	if(document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML)) {
		document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML).checked=true;
	}
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, acaid, ptres){

	if(!ptres) {
		alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
		return false;
	}


	if(dados.checked) {

	var linhaTbl = dados.parentNode.parentNode;
	var tabelaorigem = window.opener.document.getElementById('orcamento');
	if(eval(tabelaorigem.rows.length%2)) {
		var cor = "";
	} else {
		var cor = "#DCDCDC";
	}
	var linha = tabelaorigem.insertRow(2);
	linha.id = "ptres_"+ptres;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[2].insertCell(0);
	celula1.style.textAlign = "center";
	celula1.innerHTML = ptres;
	
	var celula2 = tabelaorigem.rows[2].insertCell(1);
	celula2.style.textAlign = "left";
	celula2.innerHTML = linhaTbl.cells[2].innerHTML+"<input type='hidden' name='acaid["+ptres+"]' value='"+acaid+"'>";

	var celula3 = tabelaorigem.rows[2].insertCell(2);
	celula3.style.textAlign = "right";
	celula3.innerHTML = linhaTbl.cells[4].innerHTML;
	
	var celula4 = tabelaorigem.rows[2].insertCell(3);
	celula4.style.textAlign = "right";
	celula4.innerHTML = linhaTbl.cells[5].innerHTML;
	
	var celula5 = tabelaorigem.rows[2].insertCell(4);
	celula5.style.textAlign = "right";
	celula5.innerHTML = "<a href=javascript:detfin('"+ptres+"')>"+linhaTbl.cells[6].innerHTML+"</a>";
	
	var celula6 = tabelaorigem.rows[2].insertCell(5);
	celula6.style.textAlign = "right";
	celula6.innerHTML = linhaTbl.cells[7].innerHTML;

	var celula7 = tabelaorigem.rows[2].insertCell(6);
	celula7.style.textAlign = "center";
	celula7.innerHTML = linhaTbl.cells[8].innerHTML;

        var celula8 = tabelaorigem.rows[2].insertCell(7);
	celula8.style.textAlign = "center";
	celula8.innerHTML = linhaTbl.cells[9].innerHTML;
        
        var celula9 = tabelaorigem.rows[2].insertCell(8);
	celula9.style.textAlign = "center";
	celula9.innerHTML = linhaTbl.cells[10].innerHTML;
        
        var celula10 = tabelaorigem.rows[2].insertCell(9);
	celula10.style.textAlign = "center";
	celula10.innerHTML = "<input type=\"text\" name=\"plivalor["+ptres+"]["+acaid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptres+"','0');\"  class=\"normal somar\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";
        

	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptres_'+ptres).rowIndex);
	window.opener.calculovalorPI();

	}

}


</script>
</body>
</html>

