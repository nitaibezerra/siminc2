<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function visualizarProfessoresAtuando($dados) {
	global $db;
	
	
	if($dados['colunas']=='1') {
		
		if($dados['esfera']=='E') {
			$wh = "m2.muncod='".$dados['muncod']."'";
		} elseif($dados['esfera']=='M') {
			$wh = "m1.muncod='".$dados['muncod']."'";
		} else {
			if($dados['colunas']=='1') $wh = "m2.muncod='".$dados['muncod']."' OR m1.muncod='".$dados['muncod']."'";
		}
	
		$sql = "SELECT DISTINCT CASE WHEN p.muncod IS NOT NULL THEN m1.estuf 
					   WHEN p.estuf IS NOT NULL THEN  m2.estuf END as uf,
					   CASE WHEN p.muncod IS NOT NULL THEN m1.estuf ||' / '|| m1.mundescricao 
					        WHEN p.estuf IS NOT NULL THEN  m2.estuf ||' / '|| m2.mundescricao END as municipio,
					   CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' 
					        WHEN p.estuf IS NOT NULL THEN  'Estadual' END as rede,
					   ee.pk_cod_entidade,
					   ee.no_entidade, 
					   i.iuscpf,
					   i.iusnome, 
					   pp.pfldsc,
					   CASE WHEN i.iustipoprofessor='censo' THEN 'Bolsista'
						WHEN i.iustipoprofessor='cpflivre' THEN 'Não bolsista'
						ELSE 'Bolsista' END tipoprofessor
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod  
				INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
				LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
				LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
				LEFT JOIN educacenso_2012.tab_docente d ON trim(d.num_cpf) = trim(i.iuscpf) 
				LEFT JOIN educacenso_2012.tab_docente_disc_turma de ON de.fk_cod_docente = d.pk_cod_docente
				LEFT JOIN educacenso_2012.tab_entidade ee ON ee.pk_cod_entidade = de.fk_cod_entidade
				WHERE t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_ORIENTADORESTUDO."') AND ($wh) 
				ORDER BY pp.pfldsc, 3, i.iusnome
				";
		
		$cabecalho = array("UF","Município","Rede","Cod.INEP","Nome Escola", "CPF","Nome Completo","Cargo","Tipo de Professor");
		$soma = 'N';
	
	} elseif($dados['colunas']=='2') {
		
		if(in_array('1',$dados['filtromateriais'])) {
			$or[] = "ma.mapid IS NULL";
		}
		
		if(in_array('2',$dados['filtromateriais'])) {
			$or[] = "((ma.recebeumaterialpnbe='1' OR ma.recebeumaterialpnbe='2') AND criadocantinholeitura='3')";
		}

		$sql = "SELECT DISTINCT CASE WHEN p.muncod IS NOT NULL THEN m1.estuf 
					   WHEN p.estuf IS NOT NULL THEN  es.estuf END as uf,
					   CASE WHEN p.muncod IS NOT NULL THEN m1.estuf ||' / '|| m1.mundescricao 
					        WHEN p.estuf IS NOT NULL THEN  es.estuf ||' / '|| es.estdescricao END as municipio,
					   CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' 
					        WHEN p.estuf IS NOT NULL THEN  'Estadual' END as rede,
					   count(i.iusd),
					   i2.iusnome,
					   i2.iusemailprincipal,
					   uni.unisigla||' - '||uni.uninome as universidade
				FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod AND t.pflcod='849'
				INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
				INNER JOIN sispacto.identificacaousuario i2 ON i2.picid = p.picid 
				INNER JOIN sispacto.tipoperfil t2 ON t2.iusd = i2.iusd AND t2.pflcod='826'
				INNER JOIN sispacto.universidadecadastro unc ON unc.uncid = i2.uncid 
				INNER JOIN sispacto.universidade uni ON uni.uniid = unc.uniid 
				LEFT JOIN  sispacto.materiaisprofessores ma ON ma.iusd = i.iusd 
				LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
				LEFT JOIN territorios.estado es ON es.estuf = p.estuf  
				WHERE ".(($or)?"(".implode(" OR ",$or).")":"")."   
				GROUP BY p.muncod, p.estuf, i2.iusnome, uni.unisigla, uni.uninome, m1.estuf, es.estuf, m1.mundescricao, es.estdescricao, i2.iusemailprincipal 
				ORDER BY 3, 2, i2.iusnome";
		
		$cabecalho = array("UF","Município/Estado","Rede","Qtd Professores","Nome do Coordenador Local", "Email do Coordenador Local","Universidade");
		$soma = 'S';
		
	}
	
	if($dados['aplicar']=='xls') {
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=rel_coordenador_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=rel_coordenador_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%','');
		exit;
	} else {
		$db->monta_lista_simples($sql,$cabecalho,1000000,5,$soma,'100%','N', true, false, false, true);
	}
	
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_REQUEST['filtromateriais']) $_REQUEST['filtromateriais'] = array();
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/sispacto.js"></script>
<script>
function carregarMunicipiosPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('buscar=1&relatorio=relatpersonal_professorespacto.inc&requisicao=carregarMunicipiosPorUF&id=muncod_sim&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

function aplicarFiltros(tipo) {

	document.getElementById('aplicar').value=tipo;

	if(document.getElementById('col_1').checked) {
		if(!document.getElementById('muncod_sim')) {
			alert('É obrigatório filtrar por município');
			return false;
		}
		
		if(document.getElementById('muncod_sim').value=='') {
			alert('É obrigatório filtrar por município');
			return false;
		}
	}
	
	if(document.getElementById('col_2').checked) {
		var val = false;
		if(document.getElementById('filtromaterial_1').checked) {
			val = true;
		}
		
		if(document.getElementById('filtromaterial_2').checked) {
			val = true;
		}
		
		if(val==false) {
			alert('É necessário marcar algum dos filtros de materiais');
			return false;
		}
	
	}
	
	document.getElementById('formulario').submit();

}

<? if($_REQUEST['estuf']) : ?>
jQuery(function() {
carregarMunicipiosPorUF('<?=$_REQUEST['estuf'] ?>');
jQuery("#muncod_sim").val('<?=$_REQUEST['muncod'] ?>');
});
<? endif; ?>
</script>

<form method="post" id="formulario">
<input type="hidden" name="buscar" value="1">
<input type="hidden" name="aplicar" id="aplicar" value="1">
<input type="hidden" name="relatorio" value="relatpersonal_professorespacto.inc">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
       <tr>
        	<td class="SubTituloDireita" width="25%">Colunas</td>
        	<td>
        	<input type="radio" name="colunas" <?=((!$_REQUEST['colunas'] || $_REQUEST['colunas']=='1')?"checked":"") ?> value="1" id="col_1" onclick="document.getElementById('tr_filtromateriais').style.display='none';document.getElementById('tr_filtroescolas').style.display='';"> Análise de Escolas (UF, Município, Rede, Cod.INEP, Nome Escola, CPF, Nome Completo, Cargo e Tipo de Professor)<br/>
        	<input type="radio" name="colunas" <?=(($_REQUEST['colunas']=='2')?"checked":"") ?> value="2" id="col_2" onclick="document.getElementById('tr_filtromateriais').style.display='';document.getElementById('tr_filtroescolas').style.display='none';"> Análise de Materiais (UF, Município, Rede, Nome do Professor, Nome do Coordenador Local, E-mail do Coordenador Local e IES Formadora)<br/>
        	</td>
        </tr>
       <tr id="tr_filtromateriais" <?=(($_REQUEST['colunas']=='2')?"":"style=\"display:none;\"") ?>>
        	<td class="SubTituloDireita" width="25%">Filtros Materiais</td>
        	<td>
        	<input type="checkbox" id="filtromaterial_1" name="filtromateriais[]" <?=((in_array('1',$_REQUEST['filtromateriais']))?"checked":"") ?> value="1"> Professores que não responderam nada<br/>
        	<input type="checkbox" id="filtromaterial_2" name="filtromateriais[]" <?=((in_array('2',$_REQUEST['filtromateriais']))?"checked":"") ?> value="2"> Professores que receberam os materiais e não criaram o cantinho de leitura<br/>
        	</td>
        </tr>
       <tr id="tr_filtroescolas" <?=((!$_REQUEST['colunas'] || $_REQUEST['colunas']=='1')?"":"style=\"display:none;\"") ?>>
        	<td colspan="2">
        	
		       <table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" width="100%">
		       <tr>
		        	<td class="SubTituloDireita" width="25%">Esfera</td>
		        	<td>
		        	<?
		        	$esfera = array(0 => array('codigo' => 'E','descricao' => 'Estadual'),1 => array('codigo' => 'M','descricao' => 'Municipal'));
					$db->monta_combo('esfera', $esfera, 'S', 'TODOS', '', '', '', '', 'N', 'esfera');
		        	?>        	
		        	</td>
		        </tr>		
		        <tr>
		        	<td class="SubTituloDireita">UF</td>
		        	<td>
		        	<?
		        	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
					$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'N', 'estuf', false, $_REQUEST['estuf']);
		        	?>        	
		        	</td>
		        </tr>
		        <tr>
		        	<td class="SubTituloDireita">Município</td>
		        	<td id="td_municipio"></td>
		        </tr>
		        </table>
        
        	</td>
        </tr>
        <tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" onclick="aplicarFiltros('html');" name="aplicar" value="Ver HTML"> <input type="button" onclick="aplicarFiltros('xls');" name="aplicar" value="Ver XLS"> <input type="button" name="limpar" value="Limpar" onclick="window.location='sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_professorespacto.inc';"></td>
        </tr>
</table>
</form>
<? if($_REQUEST['aplicar']) : ?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td><? visualizarProfessoresAtuando($_REQUEST); ?></td>
		</tr>
</table>
<? endif; ?>