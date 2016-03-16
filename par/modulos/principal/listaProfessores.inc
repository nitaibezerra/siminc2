<?php


define("ANO_CENSO", "2009");

/*
 * Função para buscar dados dos professores (utilizado para inserir no BeautyTips)
 */
function buscarDadosProfessores($dados) {
	global $db;
	
	$sql = "SELECT a.pk_cod_docente, a.num_cpf, a.no_docente, m.mundescricao, to_char(dt_nascimento, 'dd/mm/YYYY') as dt_nascimento,
				   CASE WHEN a.tp_sexo='1' THEN 'M' ELSE 'F' END as tp_sexo, c.no_escolaridade  
			FROM censo.tab_docente a 
			LEFT JOIN territorios.municipio m ON m.muncod=a.fk_cod_municipio_dend::character(7)  
			LEFT JOIN censo.tab_escolaridade c ON c.pk_cod_escolaridade=a.fk_cod_escolaridade 
			WHERE a.num_cpf='".$dados['cpf']."'";
	
	$tab_docente = $db->pegaLinha($sql);
	$html .= "<table class=listagem width=100%>";
	$html .= "<tr><td class=SubTituloDireita style=cursor:pointer; colspan=2 onclick=$('#celula".$tab_docente['num_cpf']."').btOff();>Fechar</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>CPF:</td><td>".$tab_docente['num_cpf']."</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>Nome:</td><td>".$tab_docente['no_docente']."</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>Município:</td><td>".$tab_docente['mundescricao']."</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>Data nascimento:</td><td>".$tab_docente['dt_nascimento']."</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>Sexo:</td><td>".$tab_docente['tp_sexo']."</td></tr>";
	$html .= "<tr><td class=SubTituloDireita>Escolaridade:</td><td>".$tab_docente['no_escolaridade']."</td></tr>";
	
	$html .= "<tr><td class=SubTituloCentro colspan=2>Formação do professor</td></tr>";
	$html .= "<tr><td colspan=2>";
	$html .= "<table width=100%>";
	$html .= "<tr><td class=SubTituloCentro>Área</td><td class=SubTituloCentro>Classe do curso</td><td class=SubTituloCentro>Licenciatura</td></tr>";
	
	
	$sql = "SELECT b.no_nome_area_ocde, a.id_licenciatura, c.no_classe_curso FROM censo.tab_docente_form_sup a
			LEFT JOIN censo.tab_area_ocde b ON b.pk_cod_area_ocde=a.fk_cod_area_ocde 
			LEFT JOIN censo.tab_classe_curso c ON c.pk_cod_classe_curso=b.fk_classe_curso   
			WHERE a.fk_cod_docente='".$tab_docente['pk_cod_docente']."'";
	
	$formacoes = $db->carregar($sql);
	
	if($formacoes[0]) {
		foreach($formacoes as $form) {
			$html .= "<tr><td>".$form['no_nome_area_ocde']."</td><td>".$form['no_classe_curso']."</td><td>".(($form['id_licenciatura'])?"Sim":"Não")."</td></tr>";			
		}
	} else {
		$html .= "<tr><td colspan=4>Não existe formação cadastrada</td></tr>";
	}
	
	$html .= "</table>";
	$html .= "</td></tr>";
	

	$html .= "<tr><td class=SubTituloCentro colspan=2>Atuação do professor</td></tr>";
	$html .= "<tr><td colspan=2>";
	$html .= "<table width=100%>";
	$html .= "<tr><td class=SubTituloCentro>Escola</td><td class=SubTituloCentro>Função</td><td class=SubTituloCentro>Turma</td><td class=SubTituloCentro>Disciplina</td></tr>";
	
	$sql = "SELECT b.no_entidade, c.no_disciplina, t.no_turma, CASE WHEN id_tipo_docente=1 THEN 'Docente' ELSE 'Auxiliar' END as tipodocente FROM censo.tab_docente_disc_turma a
			LEFT JOIN censo.tab_entidade b ON b.pk_cod_entidade=a.fk_cod_entidade 
			LEFT JOIN censo.tab_disciplina c ON c.pk_cod_disciplina=a.fk_cod_disciplina 
			LEFT JOIN censo.tab_turma t ON t.fk_cod_entidade=a.fk_cod_entidade AND t.pk_cod_turma=a.fk_cod_turma AND t.fk_ano_censo = a.fk_ano_censo 
			LEFT JOIN censo.tab_dado_docencia cc ON cc.fk_cod_entidade=a.fk_cod_entidade AND cc.fk_cod_turma=a.fk_cod_turma AND cc.pk_cod_dado_docencia=a.fk_cod_dado_docencia AND cc.fk_ano_censo=a.fk_ano_censo
			WHERE a.fk_cod_docente='".$tab_docente['pk_cod_docente']."' AND a.fk_ano_censo='".ANO_CENSO."'";
	
	$atuacoes = $db->carregar($sql);
	
	if($atuacoes[0]) {
		foreach($atuacoes as $atua) {
			$html .= "<tr><td>".$atua['no_entidade']."</td><td>".$atua['tipodocente']."</td><td>".$atua['no_turma']."</td><td>".$atua['no_disciplina']."</td></tr>";
		}
	} else {
		$html .= "<tr><td colspan=4>Não existe formação cadastrada</td></tr>";
	}

	$html .= "</table>";
	$html .= "</td></tr>";
	
	$html .= "<tr><td class=SubTituloDireita>Curso:</td><td>";
	
	$curcod = $db->pegaUm("SELECT curcod FROM par.censo_atribuicao WHERE num_cpf='".$tab_docente['num_cpf']."'");
	
	$sql = "SELECT COALESCE(pcpregiao,'')||COALESCE(pcpuf,'')||COALESCE(pcprespons,'')||COALESCE(pcpcodigoinep,'')||COALESCE(pcpdigitoinep,'')||COALESCE(ptcid,0)||COALESCE(pstid,0)||COALESCE(pmcid,0)||COALESCE(pficodigo,0) as codigo, 
				   COALESCE(pcpregiao,'')||COALESCE(pcpuf,'')||COALESCE(pcprespons,'')||COALESCE(pcpcodigoinep,'')||COALESCE(pcpdigitoinep,'')||COALESCE(ptcid,0)||COALESCE(pstid,0)||COALESCE(pmcid,0)||COALESCE(pficodigo,0) as descricao 
				   FROM par.pfcursoparfor pf 
				   GROUP BY pcpregiao,pcpuf,pcprespons,pcpcodigoinep,pcpdigitoinep,ptcid,pstid,pmcid,pficodigo";
	
	$html .= $db->monta_combo('curcod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'curcod', true, $curcod);
	
	$html .= " <input type=button name=aplicarcurso value=Aplicar value=Aplicar onclick=aplicarCurso('".$tab_docente['num_cpf']."',document.getElementById('curcod').value);></td></tr>";
	
	
	
	$html .= "</table>";
	
	echo $html;
}
/*
 * Atribuir um curso a diversos professores
 */
function aplicarcurso($dados) {
	global $db;
	
	$curcod_existe = $db->pegaUm("SELECT curcod FROM par.censo_atribuicao WHERe num_cpf='".$dados['cpf']."'");
	
	if($curcod_existe) {

		$sql = "UPDATE par.censo_atribuicao
   				SET curcod='".$dados['curcod']."'
 				WHERE num_cpf='".$dados['cpf']."'";
		
		$db->executar($sql);
		
		$db->commit();
		
	} else {
	
		$sql = "INSERT INTO par.censo_atribuicao(num_cpf, curcod, ceaano)
	    		VALUES ('".$dados['cpf']."', '".$dados['curcod']."', '".date("Y")."');";
				
		$db->executar($sql);
				
		$sql = "INSERT INTO par.censo_historico_docente(
													            pk_cod_docente, fk_cod_municipio_dend, fk_cod_estado_dnasc, fk_cod_pais_origem, 
													            fk_cod_orgao_emissor, fk_cod_estado_rg, fk_cod_municipio_dnasc, 
													            fk_cod_estado_certidao_civil, fk_cod_escolaridade, fk_cod_estado_dend, 
													            no_docente, no_email, desc_endereco, num_endereco, desc_endereco_complemento, 
													            desc_endereco_bairro, num_endereco_cep, dt_nascimento, tp_sexo, 
													            tp_cor_raca, tp_nacionalidade, no_mae, num_rg, num_rg_complemento, 
													            dt_emissao_rg, num_cpf, id_certidao_civil, no_cartorio, num_termo_certidao_civil, 
													            num_folha_certidao_civil, num_livro_certidao_civil, dt_emissao_certidao_civil, 
													            id_pos_graduacao_nenhum, id_mestrado, id_doutorado, id_especializacao, 
													            id_especifico_creche, id_especifico_pre_escola, id_especifico_nec_esp, 
													            id_especifico_ed_indigena, id_especifico_nenhum, id_intercultural_outros, 
													            num_doc_estrangeiro, num_nis_docente, desc_identificacao_migracao, 
													            cod_docente_pac, dt_cadastro, dt_ultima_alteracao, no_login_banco, 
													            num_ip_login_banco)
						SELECT * FROM censo.tab_docente WHERE num_cpf='".$dados['cpf']."'";
				
		$db->executar($sql);
				
		$pk_cod_docente = $db->pegaUm("SELECT pk_cod_docente FROM censo.tab_docente WHERE num_cpf='".$cpf."'");
				
		$sql = "INSERT INTO par.censo_historico_docente_disc_turma(
	            															fk_ano_censo, fk_cod_entidade, fk_cod_turma, fk_cod_disciplina, 
	            															fk_cod_docente, fk_cod_dado_docencia)
	    		SELECT * FROM censo.tab_docente_disc_turma WHERE fk_cod_docente='".$pk_cod_docente."'";
				
		$db->executar($sql);
				
		$sql = "INSERT INTO censo.tab_docente_form_sup(
													            fk_cod_docente, fk_cod_area_ocde, fk_cod_ies, id_licenciatura, 
													            nu_ano_conclusao, id_tipo_instituicao) 
					    SELECT * FROM censo.tab_docente_form_sup WHERE fk_cod_docente='".$pk_cod_docente."'";
				
		$db->executar($sql);
				
				
		$db->commit();
		
	}
		

}

// Se não tiver 
if(!$_SESSION['par']['muncod']) {
	
	die("<script>
			alert('Nenhum município selecionado');
			window.location='par.php?modulo=principal/listaMunicipios&acao=A';
		 </script>");
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


echo montarAbasArray( criaAbaParApoio(), "par.php?modulo=principal/listaProfessores&acao=A" );
	
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<?
// cabeçalho semelhante ao utilizado no arquivo principal/planoTrabalho
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo('PARFor ('.$nmEntidadePar.')', 'Filtros');

?>
<form method="post" name="formbusca" action="par.php?modulo=principal/listaProfessores&acao=A" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>CPF:</td>
	<td><? echo campo_texto('num_cpf', "N", "S", "CPF", 20, 150, "###.###.###-##", "", '', '', 0, '' ); ?></td>
</tr>
<tr>
	<td class='SubTituloDireita'>Nome:</td>
	<td><? echo campo_texto('no_docente', "N", "S", "Nome", 30, 150, "", "", '', '', 0, '' ); ?></td>
</tr>
<tr>
	<td class='SubTituloDireita'>Filtro de busca:</td>
	<td>
	<input type="checkbox" name="filtrop" value="FD" <? echo (($_REQUEST['filtrop']=="FD")?"checked":""); ?> > Formação diferente das turmas lecionadas
	</td>
</tr>
<tr>
	<td class='SubTituloDireita' colspan="2"><input type="submit" name="buscar" value="Buscar" onclick="divCarregando('formulario');"> <input type="button" name="todos" value="Ver todos" onclick="divCarregando('formulario');window.location='par.php?modulo=principal/listaProfessores&acao=A&muncod=<? echo $_REQUEST['muncod'] ?>';"> <input type="button" name="voltar" value="Voltar" onclick="window.location='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';"></td>
</tr>
</table>
</form>

<?

if(trim($_POST['num_cpf'])) {
	$where .= "and a.num_cpf='".str_replace(array(".","-"),"",$_POST['num_cpf'])."' ";
}

if(trim($_POST['no_docente'])) {
	$where .= "and a.no_docente ilike '%'||removeacento('".$_POST['no_docente']."')||'%' ";
}

switch($_POST['filtrop']) {
	case 'FD':
		$where .= "and f.pdfid is null";
		$join  .= "left join censo.tab_docente_form_sup d on d.fk_cod_docente = a.pk_cod_docente 
				   inner join censo.tab_docente_disc_turma e on e.fk_cod_docente = a.pk_cod_docente 
				   left join par.pfdisciplinaformacao f on f.pdfdisciplina = e.fk_cod_disciplina and f.pdfcurso = d.fk_cod_area_ocde";
		$group .= "group by a.num_cpf, 
			   				a.no_docente,
			   				b.curcod";
		break;
	
}







$sql = "select '<center><img src=../imagens/consultar.gif border=0 style=cursor:pointer; id=celula'|| a.num_cpf ||'></center>
			    <script>$(\'#celula'|| a.num_cpf ||'\').bt({ajaxPath: \'par.php?modulo=principal/listaProfessores&acao=A&requisicao=buscarDadosProfessores&cpf='||a.num_cpf||'\',trigger:\'click\',width:800,fill:\'#FFFFFF\',});</script>' as op2,
			   a.num_cpf as cpf, 
			   a.no_docente as nome,
			   b.curcod as curso
		from censo.tab_docente a 
		".$join."
		left join par.censo_atribuicao b on a.num_cpf=b.num_cpf
		where fk_cod_municipio_dend='".$_SESSION['par']['muncod']."' and a.num_cpf != '' ".$where." 
		".$group."
		order by a.no_docente";

$cabecalho = array("", "CPF", "Nome", "Curso atribuido");

$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");

?>
<script type="text/javascript">

function verDadosProfessores(cpf) {
	$('#celula'+cpf).btOn();
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url) {
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(690,400);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}
function displayStaticMessage(messageContent,cssClass) {
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(600,150);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}
function closeMessage() {
	messageObj.close();	
}

function aplicarCurso(cpf, curcod) {
    divCarregando('celula'+cpf);
	jQuery.post('par.php?modulo=principal/listaProfessores&acao=A&requisicao=aplicarcurso&cpf='+cpf+'&curcod='+curcod, function(data) {
	  var obj = document.getElementById('celula'+cpf);
	  obj.checked  = true;
	  obj.disabled = true;
	  obj.parentNode.parentNode.parentNode.cells[3].innerHTML = curcod;
	  $('#celula'+cpf).btOff();
   	  divCarregado('celula'+cpf);
	});

}

function selecionarCurso(curid) {
	document.getElementById('curid').value=curid;
}
</script>