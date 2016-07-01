<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
		<title>Dados do Dirigente - Módulo Acadêmico</title>
		<script language="JavaScript" src="../includes/calendario.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/entidades.js"></script>
		<script language="JavaScript" src="./js/rehuf.js"></script>
		<script type="text/javascript" src="/includes/estouvivo.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript">
			this._closeWindows = false;
		</script>
	</head>
	<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
		<div>
		<?php
			require_once APPRAIZ . "includes/entidades.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/TipoClassificacao.php";
			require_once APPRAIZ . "includes/ActiveRecord/classes/TipoLocalizacao.php";
		
			switch($_REQUEST['acao']) {
				case 'A';
					$_FUNCOESAUTORIZADAS = array('salvarRegistroDirigente'=>true,
												 'salvarFormacaoDirigentes'=>true);
				break;
			}
			
			if($_REQUEST['requisicao']) {
				if( $_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']] ) {
					$_REQUEST['requisicao']($_REQUEST);
				}
			} elseif($_REQUEST['opt']=="buscarCnpj") {
				buscarCnpj($_REQUEST);
			}
		
			if($_REQUEST['funid']) {
				$_SESSION['academico']['funid'] = $_REQUEST['funid'];
			}
			
			/* Verifica se o dirigente esta vinculado a uma função, caso não, imprimir erro */
			if( $_SESSION['academico']['funid'] ) {
				$ent = new Entidade();
			
				// Verifica se existe o id e se a busca foi feita por CPF (caso sim, irá carregar os dados completos)
				if($_REQUEST['busca']=='entnumcpfcnpj' && $_REQUEST['entid']) {
					$ent = $ent->carregar($_REQUEST['entid']);
				} else {
				
					// caso não, irá procurar por algum dirigente naquela função naquele hospital
					// se não encontrar, traz vazio  
					$ent = $ent->carregarEntidassociadoPorEntidade($_SESSION['academico']['entid'],$_REQUEST['funid']);
				}
				
				// configura a função
				$ent->funid = $_SESSION['academico']['funid'];
				$editavel = true;
				
				// Imprimindo a descrição da função
				echo "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>" 
					."	<tr><td class='SubTituloDireita'>Função :</td><td>".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$ent->funid."'")."</td></tr>"
					."</table>";
					
				// Imprime o formulario de edição/inserção de entidade do tipo fisico
				echo formEntidade($ent, 'academico.php?modulo=principal/editardirigente&acao=A&requisicao=salvarRegistroDirigente', PESSOA_FISICA, true, false, false, $editavel, array(), false);
				echo "<br />";
			}	
		?>
		
		<script type="text/javascript"><!--
		$('frmEntidade').onsubmit  = function(e) {
			if (trim($F('entnumcpfcnpj')) == '') {
				alert('CPF é obrigatório.');
		    	return false;
			}
			if (trim($F('entnome')) == '') {
				alert('O nome da entidade é obrigatório.');
				return false;
			}
			if (trim($F('entdatanasc')) != '') {
				if(!validaData(document.getElementById('entdatanasc'))) {
					alert("Data de nascimento é inválida.");return false;
				}
			}
			if (($('entcodent') && trim($F('entcodent')) == '') && ($('entnumcpfcnpj') && trim($F('entnumcpfcnpj')) == '')) {
				alert('Você deve preencher pelo menos um dos campos abaixo:\nCódigo INEP OU CPNJ.');
				return false;
			}
			Form.enable('frmEntidade');
			return true;
		}
		</script>
		</div>
	</body>
</html>