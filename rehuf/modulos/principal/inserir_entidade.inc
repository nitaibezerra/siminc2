<?php
require_once APPRAIZ . "includes/entidades.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoClassificacao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoLocalizacao.php";

/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

/* Verifica se tem que rodar alguma ação/requisição */
if($_REQUEST['alterabd']) {
	$_REQUEST['alterabd']($_REQUEST);
}
/* FIM - Verifica se tem que rodar alguma ação/requisição */

/* Imprimindo cabeçalho */
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
/* FIM - Imprimindo cabeçalho */

/*
 * Este arquivo gerencia 3 TELAS dentro da Entidade HOSPITAL.
 * TELA 1 : Dados do hospital (page=ent)
 * - Gerenciamento dos dados do hospital (juridico e endereço) 
 * TELA 2 : Dados do dirigente (page=dir)
 * - Gerenciamento dos dados dos dirigentes do hospital (diversos cargos) e a formação destes dirigentes.
 * TELA 3 : Dados especifícos
 * - Gerenciamento de OUTROS dados do HOSPITAL 
 */

/* Caso não seja passado nenhum paramêtro de escolha da TELA, po default será a TELA 1 */
if(!$_REQUEST['page']) {
	$_REQUEST['page'] = 'ent';	
}
/* FIM - opção default de tela*/

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/inserir_entidade&acao=A&page=".$_REQUEST['page']);

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

// verificando em qual página o programa deve ir
switch($_REQUEST['page']) {
	/* Tela de controle dos dirigentes */
	case 'dir':
		echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
		echo "<tr><td colspan='2'><strong>Dados dos dirigentes</strong></td></tr>";
		// selecionando todas as funções previstas para os hospitais (_constantes.php) e agregando com os dirigente cadastrados
		$sql = "SELECT fun.funid, fun.fundsc, ent.entnome FROM entidade.funcao fun 
				LEFT JOIN entidade.entidade ent ON fun.funid = ent.funid AND ent.entidassociado='".$_SESSION['rehuf_var']['entid']."' 
				WHERE fun.funid IN('".implode("','",$_funcoes)."')";
		$funcoeshospit = $db->carregar($sql);
		if($funcoeshospit[0]) {
			foreach($funcoeshospit as $funent) {
				echo "<tr>";
				echo "<td class='SubTituloDireita'>". $funent['fundsc'] ." :</td>";
				echo "<td><a href=\"javascript:void(0);\" onclick=\"window.open('?modulo=principal/editardirigente&acao=A&funid=".$funent['funid']."','Dirigentes','scrollbars=yes,height=600,width=600,status=no,toolbar=no,menubar=no,location=no');\">". (($funent['entnome'])?"<img src='../imagens/alterar.gif' border='0'> ".$funent['entnome']:"<img src='../imagens/gif_inclui.gif' border='0'> Não cadastrado") ."</td>";
				echo "</tr>";
			}
		} else {
			echo "<tr><td class='SubTituloEsquerda' colspan='2'>Problemas nas funções dos hospitais (_constantes.php)</td></tr>";
		}
		echo "</table>";
	break;
	/* FIM - Tela de controle dos dirigentes */
	/* Tela de controle dos dados dos hospitais */
	case 'ent':
		if (!$_SESSION['rehuf_var']['entid'] || $_SESSION['rehuf_var']['entid'] == '') {
    		$ent      = new Entidade();
    		$end      = new Endereco();
    		$editavel = false;
		} else {
    		$ent      = new Entidade($_SESSION['rehuf_var']['entid']);
    		$end      = $ent->carregarEnderecos();
    		$editavel = true;
	
    		if ($end[0] instanceof Endereco)
        		$end = $end[0];
    		else
        		$end = new Endereco();
		}
		echo formEntidade($ent, 'rehuf.php?modulo=principal/inserir_entidade&acao=A&alterabd=salvarRegistroEntidade', PESSOA_JURIDICA, true, true, false, $editavel, array(), false);
		$campos = 'Form.disable("frmEntidade");';
		?>
		<script type="text/javascript">
		$('tr_entcodent_container').style.display = 'none';
		$('tr_entcodent_container_radios').style.display = 'none';
		$('tr_entnuninsest_container').style.display = 'none';
		$('tr_entescolanova_container').style.display = 'none';
		
		
		$('frmEntidade').onsubmit  = function(e) {
			if (trim($F('entnome')) == '') {
    		alert('O nome da entidade é obrigatório.');
        	return false;
    		}
    		if (($('entcodent') && trim($F('entcodent')) == '') && ($('entnumcpfcnpj') && trim($F('entnumcpfcnpj')) == '')) {
	    		alert('Você deve preencher pelo menos um dos campos abaixo:\nCódigo INEP OU CPNJ.');
        		return false;
    		}
    		Form.enable('frmEntidade');
   			return true;
		}
		if ($('tpcid') && $('tpctgid_container')) {
			$('tpcid').onchange = function(e) {
    			if (this.value == 4) {
	                Element.show('tpctgid_container');
	            } else {
	                Element.hide('tpctgid_container');
	            }
	        }
        }
		<?
        echo $campos;
		?>

        // Ativa os campos requisitados.
        $('entemail',
          'njuid',
          'entnumdddcomercial',
          'entnumcomercial',
          'entnumramalcomercial',
          'entnumdddresidencial',
          'entnumresidencial',
          'entnumdddfax',
          'entnumfax',
          'entnumramalfax',
          'endlog',
          'endcep',
          'endlog',
          'endbai',
          'endnum',
          'endcom',
          'endlogradouro',
          'sitid',
          'edtdsc',
          'edtinfoadicional',
          'entobs',

          //botoes
          'closeWindow',
          'submitEntidade',
          'resetEntidade',
          'submitEntidadeDetalhes',
          'resetEntidadeDetalhes',
          'closeWindowDetalhes'
          ).each(
            function(a)
            {
                if (a)
                    a.removeAttribute('disabled');
            }
        );
    	</script>
		<?
	break;
	/* FIM - Tela de controle da entidade */
	/* Tela de gerenciamento dos dados especificos */
	case 'esp':
	// Buscando os dados especificos do HOSPITAL
	$dadosestruturaunidade = $db->pegaLinha("SELECT * FROM rehuf.estruturaunidade WHERE entid = '".$_SESSION['rehuf_var']['entid']."'");
	$esutipo 			= $dadosestruturaunidade['esutipo'];
	$esuareacontruida   = number_format($dadosestruturaunidade['esuareacontruida'],2,',','.');
	$esucontratualizado = (($dadosestruturaunidade['esucontratualizado']=="t")?"sim":"nao");
	$esucertiticado 	= (($dadosestruturaunidade['esucertiticado']=="t")?"sim":"nao");
	$esugestao 			= $dadosestruturaunidade['esugestao'];
	?>
	<form name='formulario' action='?modulo=principal/inserir_entidade&acao=A' onsubmit='document.getElementById("btnsubmit").disabled=true;' method='post'>
	<input type='hidden' name='alterabd' value='atualizarestruturaunidade'>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td colspan='2'><strong>Dados gerais do hospital</strong></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Tipo do hospital :</td>
		<td> 
		<? 
		$thosp = array(0 => array("codigo" => "E", "descricao" => "Especialidade"), 
					   1 => array("codigo" => "G", "descricao" => "Geral"), 
					   2 => array("codigo" => "M", "descricao" => "Maternidade")
					   );
		echo $db->monta_combo('esutipo', $thosp, 'S', 'Selecione', '', '', '', '200', 'N', 'esutipo'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Área construída total :</td>
		<td>
		<? 
		echo campo_texto('esuareacontruida', 'N', 'S', 'Área contruída total', 37, 37, "###.###.###,##", "", '', '', 0, '' ); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Contratualizado :</td>
		<td>
		<? 
		$contr = array(0 => array("codigo" => "sim", "descricao" => "Sim"), 
					   1 => array("codigo" => "nao", "descricao" => "Não") 
					   );
		echo $db->monta_combo('esucontratualizado', $contr, 'S', 'Selecione', '', '', '', '200', 'N', 'esucontratualizado'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Certificado :</td>
		<td>
		<? 
		$certi = array(0 => array("codigo" => "sim", "descricao" => "Sim"), 
					   1 => array("codigo" => "nao", "descricao" => "Não") 
					   );
		echo $db->monta_combo('esucertiticado', $certi, 'S', 'Selecione', '', '', '', '200', 'N', 'esucertiticado'); 
		?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Gestão :</td>
		<td>
		<? 
		$gesta = array(0 => array("codigo" => "E", "descricao" => "Estadual"), 
					   1 => array("codigo" => "M", "descricao" => "Municipal"),
					   2 => array("codigo" => "F", "descricao" => "Federal") 
					   );
		echo $db->monta_combo('esugestao', $gesta, 'S', 'Selecione', '', '', '', '200', 'N', 'esugestao'); 
		?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan='2' align='center'><input type='submit' id='btnsubmit' value='Salvar'></td>
	</tr>
	</table>
	</form>
	<?
	break;
	/* FIM - Tela de gerenciamento dos dados especificos */
}
?>
