<?php
    if($_REQUEST['lista']){
        switch ($_REQUEST['lista']) {
            case 'desafio':
                $cabecalho = array ("Selecione", "Desafio" );
				$titulo = "Selecione o(s) desafio(s)";
				$label = "Desafio";
				$filtro = "(des.desnome ILIKE '%{$_POST['filtroPesquisa']}%' OR ope.openome ILIKE '%{$_POST['filtroPesquisa']}%')";
                $sql_lista = "SELECT 
                                    '<input type=\"checkbox\" id=\"chk_'||desid||'\" onclick=\"resultado(this,\''||desid||'\',\''||desnome||'\');\">' as checkbox,
								des.desnome || '<br><b>Objetivo: ' || ope.openome || '</b>'
							FROM pde.ae_desafio des
							INNER JOIN pde.ae_objetivope ope ON ope.opeid = des.opeid
							". ($_POST['filtroPesquisa'] ? 'WHERE '. $filtro : '') . "
							ORDER BY des.desnome, ope.openome";

                break;
            case 'meta_pne':
				$titulo = "Selecione a(s) meta(s) PNE";
				$label = "Meta PNE";
                $cabecalho = array ("Selecione", "Metas" );
				$filtro = "mpnenome ILIKE '%{$_POST['filtroPesquisa']}%'";
                $sql_lista = "SELECT 
                                    '<input type=\"checkbox\" id=\"chk_'||mpneid||'\" onclick=\"resultado(this,\''||mpneid||'\',\''||mpnenome||'\');\">' as checkbox,
                                    mpnenome
                            FROM pde.ae_metapne
                            WHERE temid = {$_SESSION['temid']}
							". ($_POST['filtroPesquisa'] ? 'AND '. $filtro : '') . "
							ORDER BY mpnenome";

                break;
            case 'objetivoestrategico':
				$titulo = "Selecione o(s) Objetivos(s) Estratégico(s)";
				$label = "Objetivo Estratégico";
                $cabecalho = array ("Selecione", "Objetivos Estratégicos" );
				$filtro = "obenome ILIKE '%{$_POST['filtroPesquisa']}%'";
                $sql_lista = "SELECT
                                    '<input type=\"checkbox\" id=\"chk_'||obeid||'\" onclick=\"resultado(this,\''||obeid||'\',\''||obenome||'\');\">' as checkbox,
                                    obenome
                            FROM pde.ae_objetivoestrategico
                            WHERE temid = {$_SESSION['temid']}
							". ($_POST['filtroPesquisa'] ? 'AND '. $filtro : '') . "
							ORDER BY obenome";

                break;
            case 'meta_ppa':
				$titulo = "Selecione a(s) meta(s) PPA";
				$label = "Meta PPA";
                $cabecalho = array ("Selecione", "Metas" );
				$filtro = "mppanome ILIKE '%{$_POST['filtroPesquisa']}%'";
                $sql_lista = "SELECT 
                                    '<input type=\"checkbox\" id=\"chk_'||mppa.mppaid||'\" onclick=\"resultado(this,\''||mppa.mppaid||'\',\''||mppa.mppanome||'\');\">' as checkbox,
                                    mppanome
                            FROM pde.ae_metappa mppa
                            INNER JOIN pde.ae_objetivoppa obj ON obj.objid = mppa.objid
                            WHERE obj.temid = {$_SESSION['temid']}
							". ($_POST['filtroPesquisa'] ? 'AND '. $filtro : '') . "
							ORDER BY mppa.mppanome";

                break;
            case 'secretaria':
				$titulo = "Selecione a(s) secretaria(s)";
				$label = "Secretaria";
                $cabecalho = array ("Selecione", "Secretaria" );
				$filtro = "sec.secdsc ILIKE '%{$_POST['filtroPesquisa']}%'";
                $sql_lista = "SELECT 
					'<input type=\"checkbox\" id=\"chk_'||sec.secid||'\" onclick=\"resultado(this,\''||sec.secid||'\',\''||sec.secdsc||'\');\">' as checkbox,
					sec.secdsc 
				FROM painel.secretaria sec
				". ($_POST['filtroPesquisa'] ? 'WHERE '. $filtro : '') . "
				ORDER BY sec.secdsc";
                break;
            default:
				$titulo = "Selecione a(s) secretaria(s)";
				$label = "Secretaria";
                $cabecalho = array ("Selecione", "Secretaria" );
				$filtro = "sec.secdsc ILIKE '%{$_POST['filtroPesquisa']}%'";
                $sql_lista = "SELECT 
					'<input type=\"checkbox\" id=\"chk_'||sec.secid||'\" onclick=\"resultado(this,\''||sec.secid||'\',\''||sec.secdsc||'\');\">' as checkbox,
					sec.secdsc 
				FROM painel.secretaria sec
				". ($_POST['filtroPesquisa'] ? 'WHERE '. $filtro : '') . "
				ORDER BY sec.secdsc";
                break;
        }
    } else {
		$titulo = "Selecione a(s) secretaria(s)";
		$label = "Secretaria";
		$cabecalho = array ("Selecione", "Secretaria" );
		$filtro = "sec.secdsc ILIKE '%{$_POST['filtroPesquisa']}%'";
		$sql_lista = "SELECT 
			'<input type=\"checkbox\" id=\"chk_'||sec.secid||'\" onclick=\"resultado(this,\''||sec.secid||'\',\''||sec.secdsc||'\');\">' as checkbox,
			sec.secdsc 
		FROM painel.secretaria sec
		". ($_POST['filtroPesquisa'] ? 'WHERE '. $filtro : '') . "
		ORDER BY sec.secdsc";
    }
//VER($sql_lista,D);
?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
var idck;
for(i=1;i<tabelaorigem.rows.length-1;i++) {
	
	idck = tabelaorigem.rows[i].id.split("_");
	
	if(document.getElementById("chk_"+idck[1])) {
		document.getElementById("chk_"+idck[1]).checked=true;
	}
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, id, desc){


	if(!desc) {
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
	var linha = tabelaorigem.insertRow(1);
	linha.id = "id_"+id;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[1].insertCell(0);
	celula1.style.textAlign = "left";
	celula1.innerHTML = "<input type='checkbox' checked id='check' onclick='resultado("+id+");'>"+desc+"<input type='hidden' name='ids[]' value='"+id+"'>";
			
	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('id_'+id).rowIndex);
	//window.opener.calculovalorPI();

	}

}
</script>
<form action="" method="post" name="form_filtro">
	<table  class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#cdcdcd">
			<td align="center" width="100%" colspan="2" height="25" ><label style="color: rgb(0, 0, 0); font-size:12px;" class="TituloTela"><?=$titulo?></label></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><?=$label?></td>
			<td>
				<input type="text" style="text-align:left;" name="filtroPesquisa" size="31" maxlength="100" value="<?php echo $_POST['filtroPesquisa']; ?>" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:33ex;"  title='' class=' normal' />
			</td>
		</tr>	
		<tr bgcolor="#cdcdcd">
			<td height="20">&nbsp;</td>
			<td><input type="submit" value="Filtrar"></td>
		</tr>
	</table>
</form>
<?php 
monta_titulo($entnome, '&nbsp');
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');
?>
<form action="" method="post" name="formulario">
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr style="background-color: #cccccc">
	<td align="right">
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
</body>
</html>

