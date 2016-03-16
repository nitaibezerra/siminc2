<script>
function download_file(arqid){
    window.open('download_file.php?arqid='+arqid, 'download', 'height=500,width=400,scrollbars=yes,top=50,left=200');
}
</script>
<?php

// topo
include_once APPPISO . 'modulos/principal/planocarreira/partialsControl/topo.inc';

gravaDadosSessao();
verificaMuncod();

$abacod_tela = '57365';
$arMnuid = array(ABA_FP_DUPLICAR_MES, ABA_FP_IMPORTAR_ARQUIVO, ABA_FP_FORM_PROFISSIONAL, ABA_FP_LISTAR_PROFIS);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );
// monta o título da tela
monta_titulo('Complementação do Piso Salarial', 'Plano de Carreira');

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST"){

	if ($_REQUEST['requisicao'] == 'salvar'){
		$resultado = $obCarreira->insereAnexos($_POST);

		echo "<script>
		          document.getElementById('aguarde').style.display = 'none';
		     </script>";

		if( $resultado == 'ok' ){
			echo '<script type="text/javascript">alert(" Operação realizada com sucesso!");</script>';
			echo "<script>window.location.href = 'pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A';</script>";
			die;
		} else if( $resultado == 'naosalvo' ) {
			echo '<script type="text/javascript">
						alert("Falha na inserção do anexos!");
						window.location.href = "pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A";
				  </script>';
			die;
		} else if( $resultado == 'naopdf' ){
			echo '<script type="text/javascript">
						alert("Falha na inserção do anexos.\nObs.: Somente arquivos com extensão .PDF poderão ser anexados..");
						window.location.href = "pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A";
				  </script>';
			die;
		} else if( $resultado == 'existe' ){
			echo '<script type="text/javascript">
						alert("Falha na inserção do anexos.\nObs.: Não é permitido a inclusão de mais de uma Lei/Decreto ou outros documentos para mesma data.");
						window.location.href = "pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A";
				  </script>';
			die;
		}
	}
	if( $_REQUEST['requisicao'] == 'excluir' ){
		$resultado = $obCarreira->excluirAnexos( $_REQUEST['arqid'] );
		echo "<script>
                  document.getElementById('aguarde').style.display = 'none';
             </script>";
		if( $resultado == 'ok' ){
			echo '<script type="text/javascript">
		    		alert("Operação realizada com sucesso!");
		    		window.location.href="pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A";
		    	  </script>';
		    die;
		} else {
			echo '<script type="text/javascript">
		    		alert("Falha na exclusão do arquivo!");
		    		window.location.href="pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A";
		    	  </script>';
		    die;
		}
	}
	if( $_REQUEST['requisicao'] == 'download' ){
		$obCarreira->downloadAnexo( $_REQUEST['arqid'] );
		echo"<script>window.location.href = 'pisosalarial.php?modulo=principal/planocarreira/adicionar&acao=A';</script>";
	    exit;
	}
}

//monta o cabeçalho
montaCabecalho();
// view
include_once APPPISO . 'modulos/principal/planocarreira/views/formDefault.inc';
// view
monta_titulo('Documentos Anexados', '');
include_once APPPISO . 'modulos/principal/planocarreira/views/partialsViews/listaDocumentosAnexados.inc';
?>