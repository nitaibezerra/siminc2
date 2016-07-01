<?php

if ($_POST['ajaxAno']){
	
	$sql = " SELECT
					p.prgid AS codigo,
					p.prgdsc || '' AS descricao
				FROM
					par.programa p
				INNER JOIN	par.pfadesao pa ON pa.prgid = p.prgid
				Where p.prgstatus = 'A'
				and pa.pfaano = '".$_POST['ajaxAno']."'
				ORDER BY 2 ASC;";
	echo $db->monta_combo("prgid", $sql, 'S', "Selecione o Programa", '', '', '', '', 'N', 'prgid');

	exit;
}

if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND regcod in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
					muncod AS codigo,
					regcod ||' - '|| mundsc AS descricao
				FROM
					public.municipio
				Where munstatus = 'A'
				$stAnd
				ORDER BY 2 ASC;";
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}


if ($_POST['ajaxUfEscolas']){
		
		if($_POST['ajaxUfEscolas'] != 'XX'){
			echo "<b>Estado(s): {$_POST['ajaxUfEscolas']}</b><br>";
			$stWhere = "AND eed.estuf in ('".str_replace(",","','",$_POST['ajaxUfEscolas'])."')";
		}
		
		$sql_combo = "SELECT
						ent.entid as codigo,
						eed.estuf ||' - '|| ent.entnome as descricao
					  FROM entidade.entidade ent
					  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
					  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
					  WHERE	ent.entstatus = 'A'
					  AND ent.entcodent IS NOT NULL
					  AND ent.entnome IS NOT NULL
					  AND ent.tpcid in (3,1) 
					  
					  
					  UNION ALL
	
					SELECT
						ent.entid as codigo,
						eed.estuf ||' - '|| ent.entnome as descricao
					FROM entidade.entidade ent
					INNER JOIN entidade.endereco eed ON eed.entid = ent.entid {$stWhere}
					INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid in (".FUNID_SECRETARIA_MUNICIPAL_EDUCACAO.",".FUNID_SECRETARIA_ESTADUAL_EDUCACAO.")
					WHERE ent.entstatus = 'A'
	
					ORDER BY 2 ASC limit 10000";
		combo_popup( 'escolaf', $sql_combo, 'Selecione a(s) Escola(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}


if ($_POST['agrupador']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioProgramasResultado.inc");
	exit;
}

/*
//Quando recuperado o parâmetro enviado via Ajax, valor será testado logicamente nas segunites condições:  
if($_POST['tipo'] == 'educacao'){
	//Se " educacaoId == 1 " serão apresentadas apenas as Instituições da Educação Superior.
	if($_POST['educacaoId'] == '1'){ 
			$whereTipoEnsino  =  " WHERE ef.funid = ' " . ACA_ID_UNIVERSIDADE . " ' ";
	//Senão se " educacaoId == 2 " serão apresentadas apenas  as Instituições da Educação Profissional.
	}else if ($_POST['educacaoId'] == '2'){
			$whereTipoEnsino  =  " WHERE ef.funid = ' " . ACA_ID_ESCOLAS_TECNICAS . " ' ";
	}else if ($_POST['educacaoId'] == '3'){
			$whereTipoEnsino  =  " WHERE ef.funid = '16' ";
	}
}
*/	
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Programas', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">
function gerarRelatorio(req){
	var formulario = document.formulario;

	if (formulario.ano.value == ''){
		alert('Selecione o Ano!');
		return false;
	}

	if (formulario.prgid.value == ''){
		alert('Selecione o Programa!');
		return false;
	}

	if (formulario.esfera[0].checked == false && formulario.esfera[1].checked == false && formulario.esfera[2].checked == false){
		alert('Selecione a Esfera!');
		return false;
	}
	
	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}

	selectAllOptions( formulario.uf );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.coluna );
	selectAllOptions( formulario.formacaof );
	selectAllOptions( formulario.tvpidf );
	selectAllOptions( formulario.cursof );
	selectAllOptions( formulario.escolaf );
			
	document.getElementById('req').value = req;
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

/*
function ajaxRelatorio(){
	var formulario = document.formulario;
	var divRel 	   = document.getElementById('resultformulario');

	divRel.style.textAlign='center';
	divRel.innerHTML = 'carregando...';

	var agrupador = new Array();	
	for(i=0; i < formulario.agrupador.options.length; i++){
		agrupador[i] = formulario.agrupador.options[i].value; 
	}	
	
	var tipoensino = new Array();
	for(i=0; i < formulario.f_tipoensino.options.length; i++){
		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
	}		
	
	var param =  '&agrupador=' + agrupador + 
				 '&f_tipoensino=' + tipoensino +
				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
	 
    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
						        method:     'post',
						        parameters: param,
						        onComplete: function (res)
						        {
							    	divRel.innerHTML = res.responseText;
						        }
							});
	
}
*/


function filtraPrograma(ano){

	var td 	   = document.getElementById('td_programa');

	var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
			        method:     'post',
			        parameters: '&ajaxAno='+ano,
			        onComplete: function (res)
			        {
				    	td.innerHTML = res.responseText;
			        }
				});
	
}

function filtraMunicipio(){

		mostraMunicipio();
		
		var uf = document.getElementById('uf');
		var listauf = "";

		for(i=0;i<uf.length;i++){
			listauf = listauf + "," + uf[i].value;
		}

		if(listauf) listauf = listauf.substr(1);
		
		if(!listauf) listauf = 'XX';

		var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
				        method:     'post',
				        parameters: '&ajaxUf='+listauf,
				        onComplete: function (res)
				        {
							td_municipio.innerHTML = res.responseText;
				        }
					});

		//filta o campo escola
		var td_escolas   = document.getElementById('td_escolas');

		var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
	        method:     'post',
	        parameters: '&ajaxUfEscolas='+listauf,
	        onComplete: function (res)
	        {
				td_escolas.innerHTML = res.responseText;
	        }
		});
		
}

function mostraMunicipio(){

	//document.getElementById('agrupador').options.length=0;

	var tr_municipio   = document.getElementById('tr_municipio');
	var td_municipio   = document.getElementById('td_municipio');

	var agrupador = document.getElementById('agrupadorOrigem');
	var total = agrupador.length;

	for(i=0; i<total; i++){
		if(agrupador[i]){
			if(agrupador[i].value == 'coordenador') {
				document.getElementById('agrupadorOrigem').remove(i);
			}
		}
	}


	var agrupador = document.getElementById('agrupador');
	var total = agrupador.length;

	for(i=0; i<total; i++){
		if(agrupador[i]){
			if(agrupador[i].value == 'coordenador') {
				document.getElementById('agrupador').remove(i);
			}
		}
	}
		
	var item = document.createElement('option');
	
	if(document.formulario.esfera[1].checked == true || document.formulario.esfera[2].checked == true){
		tr_municipio.style.display = '';
		td_municipio.style.display = '';

	}	
	else{
		td_municipio.style.display = 'none';
		tr_municipio.style.display = 'none';
		
		 item.text = 'Nome do Coordenador';
		 item.value = 'coordenador';
		 
		  try {
			document.getElementById('agrupadorOrigem').add(item, null); // standards compliant; doesn't work in IE
		  }
		  catch(ex) {
			  document.getElementById('agrupadorOrigem').add(item); // IE only
		  }
		
	}
	
}



function appendOptionLast(num)
{
  var elOptNew = document.createElement('option');
  elOptNew.text = 'Append' + num;
  elOptNew.value = 'append' + num;
  var elSel = document.getElementById('selectX');

  try {
    elSel.add(elOptNew, null); // standards compliant; doesn't work in IE
  }
  catch(ex) {
    elSel.add(elOptNew); // IE only
  }
}

/*
//Quando selecionado o radiobutton em um tipo de Educação, passa por parâmetro o valor do mesmo, via Ajax.   
function selecionaEducacao(idEducacao){
	var eduId = idEducacao.value;
	var url = '?modulo=relatorio/relatorioDecreto7446&acao=A'; 
	new Ajax.Request(url,{
		method: 'post',
		parameters: '&tipo=educacao&educacaoId=' + eduId, 
			onComplete: function(res){
				res.responseText;
			}
		}
	);
}
*/
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="req" id="req" value="" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Ano</td>	
		<td>
		<?php
			$sql = " SELECT
							pfaano AS codigo,
							pfaano AS descricao
						FROM
							par.pfadesao 
						group by pfaano
						ORDER BY 2 ASC;";
			$db->monta_combo("ano", $sql, 'S', "Selecione o Ano", 'filtraPrograma', '', '', '', 'N', 'ano');
	    ?>		

		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Programa:</td>	
		<td id="td_programa">
			<select name="prgid" class="CampoEstilo" style="width: auto" id="prgid">
				<option value="">Selecione o Programa</option>
			</select>	
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Esfera:</td>	
		<td>
			<input type="radio" value="1" name="esfera" onclick="mostraMunicipio();" onchange="mostraMunicipio();">Estadual
			&nbsp;&nbsp;  
			<input type="radio" value="2" name="esfera" onclick="mostraMunicipio();" onchange="mostraMunicipio();">Municipal  		
			&nbsp;&nbsp;  
			<input type="radio" value="3" name="esfera" onclick="mostraMunicipio();" onchange="mostraMunicipio();">Estadual / Municipal
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Estado:</td>	
		<td >
			<?php
			/*
			$sql = " SELECT
							regcod AS codigo,
							descricaouf AS descricao
						FROM
							public.uf
						WHERE idpais = 1
						ORDER BY 2 ASC;";
			$db->monta_combo("uf", $sql, 'S', "Todos", 'filtraMunicipio', '', '', '', 'N', 'uf');
			*/
				$sql_combo = "SELECT
							regcod AS codigo,
							descricaouf AS descricao
						FROM
							public.uf
						WHERE idpais = 1
						ORDER BY 2 ASC;";
				combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
			
	    	?>		

		</td>
	</tr>
	<tr id="tr_municipio" style="display:none;">
		<td class="SubTituloDireita" valign="top">Município:</td>	
		<td id="td_municipio">
			<?php 
			
				$sql_combo = "SELECT
								muncod AS codigo,
								regcod ||' - '|| mundsc AS descricao
							FROM
								public.municipio
							Where munstatus = 'A'
							and regcod = 'XX'
							ORDER BY 2 ASC;";
				combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);			
			
			?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita" valign="top">Colunas <br>(Somente para os agrupadores:<br> Nome do Aluno,<br> Nome do Professor,<br> Nome do Coordenador)</td>
		<td>
			<?
			$matriz2 = colunas();
                        
			$campoAgrupador2 = new Agrupador( 'formulario' );
			$campoAgrupador2->setOrigem( 'colunaOrigem', null, $matriz2 );
			$campoAgrupador2->setDestino( 'coluna', null, null);
			$campoAgrupador2->exibir();
			?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="top">Formação</td>
		<td>
			<?
				$sql_combo = "SELECT 
								tfoid as codigo,
								tfodsc as descricao
							FROM 
								public.tipoformacao
							WHERE
								tfostatus = 'A'
							ORDER BY
								tfoordem";
				combo_popup( 'formacaof', $sql_combo, 'Selecione a(s) Formação(ões)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, '', true);
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Vínculo Empregatício</td>
		<td>
			<?
				$sql_combo = "select
								tvpid as codigo,
								tvpdsc as descricao
							from public.tipovinculoprofissional tvp
							where tvpstatus = 'A'
							order by tvpdsc";
				combo_popup( 'tvpidf', $sql_combo, 'Selecione o(s) Vínculo(s) Empregatício(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Curso</td>
		<td>
			<?
				$sql_combo = "SELECT 
								pfcid as codigo,
								pfcdescricao as descricao
							FROM 
								par.pfcurso
							WHERE
								pfcstatus = 'A'
								and pfcid not in (5)
							ORDER BY
								pfcdescricao";
				combo_popup( 'cursof', $sql_combo, 'Selecione o(s) Curso(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Escola</td>
		<td id="td_escolas">
			<?/*
				if($_SESSION['par']['itrid'] == 2){
					//$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}' AND eed.estuf = '{$_SESSION['par']['estuf']}'";
					$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}'";
				}else{
					$stWhere = "AND eed.estuf = '{$_SESSION['par']['estuf']}'";
				}
				
				$sql_combo = "SELECT
								ent.entid as codigo,
								ent.entnome as descricao
							  FROM entidade.entidade ent
							  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
							  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
							  WHERE	ent.entstatus = 'A'
							  AND ent.entcodent IS NOT NULL
							  AND ent.entnome IS NOT NULL
							  AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1)."
							  
							  
							  UNION ALL
			
							SELECT
								ent.entid as codigo,
								ent.entnome as descricao
							FROM entidade.entidade ent
							INNER JOIN entidade.endereco eed ON eed.entid = ent.entid {$stWhere}
							INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = ".($_SESSION['par']['itrid'] == 2 ? FUNID_SECRETARIA_MUNICIPAL_EDUCACAO : FUNID_SECRETARIA_ESTADUAL_EDUCACAO)."
							WHERE ent.entstatus = 'A'
			
							ORDER BY 2 ASC
							  ";	
			*/
			
			
				$sql_combo = "SELECT
								ent.entid as codigo,
								eed.estuf ||' - '|| ent.entnome as descricao
							  FROM entidade.entidade ent
							  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
							  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
							  WHERE	ent.entstatus = 'A'
							  AND ent.entcodent IS NOT NULL
							  AND ent.entnome IS NOT NULL
							  AND ent.tpcid in (3,1) 
							  
							  
							  UNION ALL
			
							SELECT
								ent.entid as codigo,
								eed.estuf ||' - '|| ent.entnome as descricao
							FROM entidade.entidade ent
							INNER JOIN entidade.endereco eed ON eed.entid = ent.entid {$stWhere}
							INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid in (".FUNID_SECRETARIA_MUNICIPAL_EDUCACAO.",".FUNID_SECRETARIA_ESTADUAL_EDUCACAO.")
							WHERE ent.entstatus = 'A'
			
							ORDER BY 2 ASC limit 10000";
				combo_popup( 'escolaf', $sql_combo, 'Selecione a(s) Escola(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, '', true);
			?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita" valign="top">Expandir Relatório</td>
		<td>
			<?
			$expandir = 0;
			$sql = "(SELECT
						1 as codigo,
						'Expandir' as descricao)
					UNION ALL
					(SELECT
						0 as codigo,
						'Não Expandir' as descricao)";	

			$db->monta_radio('expandir', $sql, '',1);
			?>
		</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
			<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
		</td>
	</tr>
</table>
</form>
</body>
<?
function agrupador(){
	return array(

				array('codigo' => 'pais',
					  'descricao' => 'País'),
				array('codigo' => 'estado',
					  'descricao' => 'Estado'),				
				array('codigo' => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' => 'curso',
					  'descricao' => 'Curso'),
				array('codigo' => 'aluno',
					  'descricao' => 'Nome do Aluno'),
				array('codigo' => 'professor',
					  'descricao' => 'Nome do Professor'),
				array('codigo' => 'coordenador',
					  'descricao' => 'Nome do Coordenador'),
				array('codigo' => 'escola',
					  'descricao' => 'Escola')
				);
}
// CPF	Nome	Email	Telefone	Formação	Vínculo	Função atual	Órgão	Cargo Efetivo	Exercício	Fase
function colunas(){
    return array(
        array('codigo' => 'cpf', 'descricao' => 'CPF'),
        array('codigo' => 'telefone', 'descricao' => 'Telefone'),
        array('codigo' => 'vinculo', 'descricao' => 'Vínculo'),
        array('codigo' => 'funcao_atual', 'descricao' => 'Função atual'),
        array('codigo' => 'orgao', 'descricao' => 'Órgão'),
        array('codigo' => 'cargo_efetivo', 'descricao' => 'Cargo Efetivo'),
        array('codigo' => 'exercicio', 'descricao' => 'Exercício'),
        array('codigo' => 'fase', 'descricao' => 'Fase'),
        array('codigo' => 'curso', 'descricao' => 'Curso'),
        array('codigo' => 'email', 'descricao' => 'E-mail'),
        array('codigo' => 'formacao', 'descricao' => 'Formação')
    );
}
?>
</html>