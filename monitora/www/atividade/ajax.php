<?php

/**
 * Centraliza as requisiчѕes ajax do mѓdulo.  
 *
 * @author Renъ de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

// carrega as funчѕes gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funчѕes de integraчуo
require_once( APPRAIZ."monitora/modulos/principal/acao/atividade/acao_atividade_funcoes.inc" );

// abre conexуo com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

// carrega as funчѕes do mѓdulo pde
include APPRAIZ . "pde/www/_constantes.php";
include APPRAIZ . "pde/www/_funcoes.php";
include APPRAIZ . "pde/www/_componentes.php";

// indica ao navegador o tipo de saэda
header( 'Content-type: text/plain' );
header( 'Cache-Control: no-store, no-cache' );

switch ( $_REQUEST['evento'] ) {

	case 'arvore_alterar_atividade':
		// verifica permissуo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para alterar a atividade.';
			exit();
		}
		if ( in_array( $_REQUEST['campo'], array( 'atidatainicio', 'atidatafim', 'atidataconclusao' ) ) ) {
			switch( $_REQUEST[ 'campo' ] ) {
				case 'atidatainicio':
					// a nova data de inicio nao pode ser posterior a data de termino
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] ,  $_REQUEST['valor'] , null, null ) ) {
						echo " A data de inэcio nуo pode ser posterior a data de tщrmino";
						exit();
					}
					break;
				case 'atidatafim':
					// a nova data de termino nao pode ser anterior a data de inicio
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , $_REQUEST['valor'], null ) ) {
						echo " A data de tщrmino nуo pode ser anterior a data de inэcio";
						exit();
					}
					break;
				case 'atidataconclusao':
					// a nova data de conclusao nao pode ser anterior a data de inico
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , null, $_REQUEST['valor'] ) ) {
						echo " A data de conclusуo nуo pode ser anterior a data de inэcio";
						exit();
					}
					break;
				default:
					break;	
				
			}
			$valor = $_REQUEST['valor'];
		} else {
			$valor = $_REQUEST['valor'];
		}
		$sql = sprintf(
			"update pde.atividade set %s = '%s' where atiid = %d",
			$_REQUEST['campo'],
			$valor,
			$_REQUEST['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		exit();

	case 'arvore_ocultar':
		arvore_ocultar_item( $_REQUEST['atiid'] );
		exit();

	case 'arvore_exibir':
		arvore_exibir_item( $_REQUEST['atiid'] );
		exit();

	case 'arvore_excluir':
		// verifica permissуo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para excluir a atividade.';
			exit();
		}
		// efetiva a exclusуo
		if( !atividade_excluir( $_REQUEST['atiid'] ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da сrvore
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_inserir':
		// verifica permissуo no pai do item novo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiidpai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para cadastrar atividade.';
			exit();
		}
		// verifica se o tэtulo foi preenchido
		$titulo = trim( $_REQUEST['atidescricao'] );
		if ( empty( $titulo ) ){
			echo 'O tэtulo щ obrigatѓrio.';
			exit();
		}
		// efetiva a inserчуo
		if ( !atividade_inserir( $_REQUEST['atiidpai'], $titulo ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza projeto da sessуo
		$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $_REQUEST['atiidpai'] );
		$_SESSION['projeto'] = (integer) $db->pegaUm( $sql );
		// atualiza dados da сrvore
		atividade_calcular_dados( $_REQUEST['atiidpai'] );
		$db->commit();
		exit();

	case 'arvore_mudar_ordem':
		// verifica permissуo no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = ". (integer) $_REQUEST['origem'];
		$atiidpai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $atiidpai, $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para alterar a atividade.';
			exit();
		}
		$ordem_origem = $db->pegaUm( "select atiordem from pde.atividade where atiid = ". (integer) $_REQUEST['origem'] );
		$ordem_destino = $db->pegaUm( "select atiordem from pde.atividade where atiid = ". (integer) $_REQUEST['destino'] );
		
		if ( !$ordem_origem || !$ordem_destino ) {
			exit();
		}
		$sql = sprintf(
			"update pde.atividade
			set atiordem = %d
			where atiid = %d",
			$ordem_destino,
			$_REQUEST['origem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro.';
			exit();
		}
		$sql = sprintf(
			"update pde.atividade
			set atiordem = %d
			where atiid = %d",
			$ordem_origem,
			$_REQUEST['destino']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro.';
			exit();
		}
		
		// atualiza dados da сrvore
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_mudar_nivel':
		// verifica permissуo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para alterar a atividade.';
			exit();
		}
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai_antigo = $db->pegaUm( $sql );
		// efetiva a mudanчa de nэvel
		$funcao = $_REQUEST['direcao'] == 'esquerda' ? 'atividade_profundidade_esquerda' : 'atividade_profundidade_direita';
		if ( !$funcao( $_REQUEST['atiid'] ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da сrvore
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai_novo = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai_antigo );
		atividade_calcular_dados( $atiidpai_novo );
		$db->commit();
		exit();

	case 'arvore_recarregar':
		// carrega a сrvore
		$lista = atividade_listar( $_REQUEST['atividade'], $_REQUEST['profundidade'], $_REQUEST['situacao'],  $_REQUEST['usuario'] );
		$diretorio = $_SESSION["sisdiretorio"] . "/atividade";
		echo arvore_corpo( $lista, $diretorio );
		exit();

	case 'arvore_mudar_pai':
		// verifica permissуo no pai do item a ser movido
		if ( !atividade_verificar_responsabilidade( $_REQUEST['pai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		$avo = $atividade["atiidpai"];
		// move para trсs os antigos irmуos
		$sql = "update pde.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		
		// muda o pai
		$sql = "update pde.atividade set atiidpai = " . (integer) $_REQUEST['pai'] . ", atiordem = ( select count(atiid) + 1 from pde.atividade where atistatus = 'A' and atiidpai = ". (integer) $_REQUEST['pai'] ." ) where atiid = " . (integer) $atividade["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		atividade_calcular_dados( $avo );
		$db->commit();
		exit();

	case 'arvore_mudar_irma':
		// verifica permissуo no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$pai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $pai, $_REQUEST['usucpf'] ) ) {
			echo 'Usuсrio sem permissуo para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		// move para trсs os antigos irmуos
		$sql = "update pde.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// captura os dados do novo irmуo
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$irma = $db->pegaLinha( $sql );
		// move os novos irmуos pra frente
		$sql = "update pde.atividade set atiordem = atiordem + 1 where atiidpai = " . (integer) $irma["atiidpai"] . " and atiordem > " . (integer) $irma["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// coloca a atividade na nova posiчуo
		$sql = "update pde.atividade set atiordem = " . ( $irma["atiordem"] + 1 ) . ", atiidpai = ". $irma["atiidpai"] ." where atiid = " . $_REQUEST["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// organiza a сrvore
		atividade_calcular_dados( $pai );
		$db->commit();
		exit();
		
	default:
		echo '';
		exit();

}

?>