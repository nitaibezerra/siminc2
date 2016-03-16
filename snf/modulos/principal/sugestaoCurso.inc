<?php

	if($_REQUEST['info_curso'] == 'S'){ 
		
		if($_REQUEST['sucid']) {
			$sucid = $_REQUEST['sucid'];
			$where .= "Where sucid = '$sucid'";
		}		
		
		echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
		echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>";		
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		$sql = "
			Select 	upper(foccurso) as foccurso, 
					upper(focdescricao) as focdescricao, 
					upper(focjustificativa) as focjustificativa
			From snf.sugestaocurso
			$where
		";
		$t = $db->pegaLinha($sql);
		$_SESSION['titulo'] = $t['foccurso']; 
		
		echo "<td class=SubTituloCentro>Informação sobre o curso: ".$_SESSION['titulo']."</td>";
		echo "<tr>";
		echo "</tr>";
		echo "<tr>";
		echo "<td>";
		
		$cabecalho = array("Curso.", "Descriçao do Curso", "Justificativa pelo Curso");
		$db->monta_lista_simples($sql, $cabecalho, 1000000, 10, 'N', '100%', $par2);
		
		echo "</td>";
		echo "</tr>";
		echo "</table>";
		exit;
	}
?>

<?php
	if($_REQUEST['requisicao']){
		$_REQUEST['requisicao']($_REQUEST);
		die;
	}

	include APPRAIZ. "includes/cabecalho.inc";
	print "<br/>";

	$abacod_tela = 57558;
	$url = 'snf.php?modulo=principal/sugestaoCurso&acao=A';
	$parametros = null;
	$arMnuid = array();
	$db->cria_aba($abacod_tela, $url, $parametros, $arMnuid);

	monta_titulo('Sugestão de Curso' , obrigatorio()." Indica campos obrigatórios.");	
?>

	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../snf/geral/snf.css" ></link>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	
	<script>
	jQuery(document).ready(function(){
		jQuery('#salvar').click(function(){
			if( document.formulario.foccurso.value == '' ){
				alert( "O campo Curso é obrigatório." );
				return false;
			}
			if( document.formulario.focdescricao.value == '' ){
				alert( "O campo Descrição é obrigatório." );
				return false;
			}
			if( document.formulario.focjustificativa.value == '' ){
				alert( "O campo Justificativa é obrigatório." );
				return false;
			}
			if( document.formulario.fexid[0].value == '' ){
				alert( "O campo Função Exercida é obrigatório." );
				return false;
			}
			if( document.formulario.pk_cod_etapa_ensino[0].value == '' ){
				alert( "O campo Etapa de ensino em que leciona é obrigatório." );
				return false;
			}
			if( document.formulario.pk_cod_disciplina[0].value == '' ){
				alert( "O campo Disciplina em que leciona é obrigatório." );
				return false;
			}
			if( document.formulario.pk_cod_mod_ensino[0].value == '' ){
				alert( "O campo Modalidade em que leciona é obrigatório." );
				return false;
			}
			
			selectAllOptions(document.formulario.fexid);
			selectAllOptions(document.formulario.pk_cod_etapa_ensino);
			selectAllOptions(document.formulario.pk_cod_disciplina);
			selectAllOptions(document.formulario.pk_cod_mod_ensino);
	
			jQuery('#requisicao').val('enviarcurso');
			jQuery('#formulario').submit();
		});
	});
	
	function informacaoCurso(sucid) {
		if(document.getElementById('info_curso').value != '') {
			var formulario = document.formulario;
			//formulario.target = 'InformacaoCurso';
			var janela = window.open('snf.php?modulo=principal/sugestaoCurso&acao=A&info_curso=S&sucid='+sucid, '', 'width=660,height=345,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.submit();
			janela.focus();
		} else {
			alert("Selecione um relatório");
			return false;
		}
	}
	
	function salvar_sugestao(){
		var sucid = jQuery('input:radio[name=sucid]:checked').val();
		if(sucid > 0){
			jQuery.ajax({
				   type: "POST",
				   url: 'snf.php?modulo=principal/sugestaoCurso&acao=A',
				   data: "requisicao=set_curso_sugestao&sucid=" + sucid,
				   success: function(msg){
				   		if(msg != "OK"){
				   			window.location.href = window.location;
				   		}else{
				   			alert('Não foi possível salvar o curso selecionado.');
				   		}
				   }
				 });
		}else{
			alert( "É necessario selecionar um curso." );
			return false;
		}
	}
	
	</script>

	<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
		<input type="hidden" name="requisicao" 	id="requisicao" value="">
		<input type="hidden" id="info_curso" name="info_curso" value="info_curso"/>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
			<tr>
				<td class="blue subtituloDireita" width="10%">Orientações</td>
				<td class="blue" colspan="2">
				<p>
					Nesta aba, aparecem as sugestões feitas pelas escolas de novos cursos que poderiam 
					aparecer como opção aos professores. Cada escola pôde sugerir apenas um curso.
					</p>
					<p>
		 			A Secretaria de Educação também pode enviar somente uma sugestão de curso ao Fórum Estadual 
		 			Permanente de Apoio à Formação Docente. Pode ser um curso escolhido entre aqueles sugeridos pela 
		 			escola ou pode-se fazer uma nova sugestão de curso.
		 			</p>
					<p>
					Caso seja feita opção por um curso sugerido por uma escola, basta selecionar e salvar. Caso a opção seja por 
					sugerir um novo curso, deve-se preencher o formulário abaixo e salvar. Neste momento, o curso vai ser incluído 
					na tabela onde constam as sugestões das escolas. Deve-se marcar o curso preenchido pela Secretaria e salvar 
					novamente.
					</p>
				</td>
			</tr>
			<tr>
				<td align="right">Nome do Curso:</td>
				<td><?php echo campo_texto("foccurso",'S','S','Nome do Curso',"95","100","","","","","","id='foccurso'","") ?></td>
			</tr>
			<tr>
				<td align="right">Descrição:</td>
				<td><?=campo_textarea('focdescricao', 'S', 'S', 'Descrição', '100', '3', '500'); ?></td>
			</tr>
			<tr>
				<td align="right">Justificativa e diferença em relação aos cursos já apresentados:</td>
				<td><?=campo_textarea('focjustificativa', 'S', 'S', 'Justificativa', '100', '5', '500'); ?></td>
			</tr>
			<tr>
				<td align="right">Público Alvo:</td>
				<td>Função Exercida:<br />
					<?php 
						$sql = "
							Select	fexid AS codigo, 
									fexdesc AS descricao
							From catalogocurso.funcaoexercida
							Where fexstatus = 'A'
						 	Order by fexdesc
						 ";
						combo_popup('fexid', $sql, 'Função', '400x400', '', '', '', 'S', '', '', 5, 500);
					?>
					
					<img border="0" src="../imagens/obrig.gif"><br/><br/>Etapa de ensino em que leciona:<br/>
					<?php 
						$sql = "
							Select	pk_cod_etapa_ensino AS codigo, 
									no_etapa_ensino AS descricao
							From educacenso_2011.tab_etapa_ensino
							order by cod_etapa_ordem
						";
						combo_popup('pk_cod_etapa_ensino', $sql, 'Etapas', '400x400', '',	'', '', 'S', '', '', 5, 500);
					?>
					
					<img border="0" src="../imagens/obrig.gif"><br/><br/>Disciplina em que leciona:<br/>
					<?php			
						$sql = "
							Select 	pk_cod_disciplina as codigo, 
									no_disciplina as descricao
							From educacenso_2011.tab_disciplina
							order by no_disciplina
						";
						combo_popup('pk_cod_disciplina', $sql, 'Disciplina', '400x400', '','', '', 'S', '', '', 5, 500);
					?>
					
					<img border="0" src="../imagens/obrig.gif"><br /><br />Modalidade em que leciona:<br />
					<?php 
						$sql = "
							Select  pk_cod_mod_ensino AS codigo, 
									no_mod_ensino AS descricao
							From educacenso_2011.tab_mod_ensino
							order by no_mod_ensino
						";
						combo_popup('pk_cod_mod_ensino', $sql, 'Modalidade', '400x400', '', '', '', 'S', '', '', 5, 500);
					
					?>
					<img border="0" src="../imagens/obrig.gif"><br /><br />
				</td>
			</tr>
		</table>			
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
			<tr>
				
				<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
				<td><input type="button" value="Voltar" onclick="history.back(-1);" />
					<!-- input type="button" id="salvar" value="Adicionar na Lista"  /-->
				</td>
			</tr>
		</table>
	</form>
<?php 
	$sql = "
		Select	Case when sucsugescolha = 'S' Then '<input type=\"radio\" checked name=\"sucid\" value=\"' || sucid || '\"/>' Else '<input type=\"radio\" name=\"sucid\" value=\"' || sucid || '\"/>' end as acao, 
				Case When pdicodinep = '' or pdicodinep isnull then '00000000' else pdicodinep end as pdicodinep, 
				Case When pdenome = '' or pdenome isnull then 'Inserido pela Secretária' else pdenome end as pdenome,
				pdiesfera,
				'<a href=\"javascript:void(0)\" onclick=\"informacaoCurso('||sucid||')\">&nbsp;'||foccurso||'</a>' as sucid
				
		From snf.sugestaocurso
		where pdiesfera = '".$_SESSION['snf']['esfera']."' and estuf = '".$_SESSION['snf']['est_uf']."' and muncod = '".$_SESSION['snf']['mun_cod']."'  
  		order by sucid desc
	";
//ver($sql, d)
	$cabecalho 		= array('Ação','Código INEP','Escola','Esfera','Curso');
	$db->monta_lista($sql, $cabecalho, 100, 10, 'N','','','','form_sugestao','','');

?>
	<!-- table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
		<tr>
			<td class="SubTituloDireita" width="20%">Salvar o curso Selecionado acima?</td>
			<td>
				<input type="button" name="salvar_sugestao" id="salvar_sugestao" value="Salvar o curso escolhido" onclick="salvar_sugestao();" />
			</td>
		</tr>
	</table -->