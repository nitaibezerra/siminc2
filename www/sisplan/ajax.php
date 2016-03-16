<?php

/**
 * Centraliza as requisi��es ajax do m�dulo.  
 *
 * @author Ren� de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	echo "Ocorreu um erro. Por favor tente mais tardassAe.";
	exit();
}


set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as fun��es do m�dulo
include '_constantes.php';
include '_funcoes.php';
include '_componentes.php';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

// indica ao navegador o tipo de sa�da
header( 'Content-type: text/plain' );
header( 'Cache-Control: no-store, no-cache' );

switch ( $_REQUEST['evento'] ) {

	case 'arvore_alterar_atividade':
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		if ( in_array( $_REQUEST['campo'], array( 'atidatainicio', 'atidatafim', 'atidataconclusao' ) ) ) {
			switch( $_REQUEST[ 'campo' ] ) {
				case 'atidatainicio':
					// a nova data de inicio nao pode ser posterior a data de termino
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] ,  $_REQUEST['valor'] , null, null ) ) {
						echo " A data de in�cio n�o pode ser posterior a data de t�rmino";
						exit();
					}
					break;
				case 'atidatafim':
					// a nova data de termino nao pode ser anterior a data de inicio
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , $_REQUEST['valor'], null ) ) {
						echo " A data de t�rmino n�o pode ser anterior a data de in�cio";
						exit();
					}
					break;
				case 'atidataconclusao':
					// a nova data de conclusao nao pode ser anterior a data de inico
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , null, $_REQUEST['valor'] ) ) {
						echo " A data de conclus�o n�o pode ser anterior a data de in�cio";
						exit();
					}
					break;
				default:
					break;

			}
			$valor = $_REQUEST['valor'];
			if ( !ereg( "^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$", $valor, $regs ) )
			{
				echo "Data inv�lida";
				exit();
			}
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
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para excluir a atividade.';
			exit();
		}
		// efetiva a exclus�o
		if( !atividade_excluir( $_REQUEST['atiid'] ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da �rvore
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_inserir':
		// verifica permiss�o no pai do item novo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiidpai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para cadastrar atividade.';
			exit();
		}
		// verifica se o t�tulo foi preenchido
		$titulo = trim( $_REQUEST['atidescricao'] );
		if ( empty( $titulo ) ){
			echo 'O t�tulo � obrigat�rio.';
			exit();
		}
		// efetiva a inser��o
		if ( !atividade_inserir( $_REQUEST['atiidpai'], $titulo ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza projeto da sess�o
		$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $_REQUEST['atiidpai'] );
		$_SESSION['projeto'] = (integer) $db->pegaUm( $sql );
		// atualiza dados da �rvore
		atividade_calcular_dados( $_REQUEST['atiidpai'] );
		$db->commit();
		exit();

	case 'arvore_mudar_ordem':
		// verifica permiss�o no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = ". (integer) $_REQUEST['origem'];
		$atiidpai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $atiidpai, $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
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

		// atualiza dados da �rvore
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_mudar_nivel':
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		$sql = "select atiidpai from pde.atividade where atiid = " . (integer) $_REQUEST['atiid'];
		$atiidpai_antigo = $db->pegaUm( $sql );
		// efetiva a mudan�a de n�vel
		$funcao = $_REQUEST['direcao'] == 'esquerda' ? 'atividade_profundidade_esquerda' : 'atividade_profundidade_direita';
		if ( !$funcao( $_REQUEST['atiid'] ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da �rvore
		$sql = "select atiidpai from pde.atividade where atiid = " . (integer) $_REQUEST['atiid'];
		$atiidpai_novo = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai_antigo );
		atividade_calcular_dados( $atiidpai_novo );
		$db->commit();
		exit();

	case 'arvore_recarregar':
		// verifica se est� em subatividades
		$sql = "select _atiprojeto from pde.atividade where atiid = ". (integer) $_REQUEST['atividade'];
		$projeto = $db->pegaUm( $sql );
		$subatividade = ( $_REQUEST['atividade'] == $projeto );
		// carrega a �rvore
		$lista = atividade_listar( $_REQUEST['atividade'], $_REQUEST['profundidade'], $_REQUEST['situacao'],  $_REQUEST['usuario'], $_REQUEST['perfil'] );
		echo arvore_corpo( $lista, null, $subatividade, $_REQUEST['numeracao_relativa'] );
		exit();

	case 'arvore_mudar_pai':
		// verifica permiss�o no pai do item a ser movido
		if ( !atividade_verificar_responsabilidade( $_REQUEST['pai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		$avo = $atividade["atiidpai"];
		// move para tr�s os antigos irm�os
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
		// verifica permiss�o no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$pai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $pai, $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		// move para tr�s os antigos irm�os
		$sql = "update pde.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// captura os dados do novo irm�o
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$irma = $db->pegaLinha( $sql );
		// move os novos irm�os pra frente
		$sql = "update pde.atividade set atiordem = atiordem + 1 where atiidpai = " . (integer) $irma["atiidpai"] . " and atiordem > " . (integer) $irma["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// coloca a atividade na nova posi��o
		$sql = "update pde.atividade set atiordem = " . ( $irma["atiordem"] + 1 ) . ", atiidpai = ". $irma["atiidpai"] ." where atiid = " . $_REQUEST["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// organiza a �rvore
		atividade_calcular_dados( $pai );
		$db->commit();
		exit();

	/*
	 * copiar informa��es gerais
	 * copiar restri��es
	 * copiar documentos
	 * copiar responsabilidades
	 * copiar observa��es
	 * copiar atividades filhas
	 */
	case 'copiar':
		
		$_REQUEST['origem'] = (integer) $_REQUEST['origem'];
		$_REQUEST['destino'] = (integer) $_REQUEST['destino'];
		
		if ( !atividade_verificar_responsabilidade( $_REQUEST['origem'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		$origem = array();
		switch ( $_REQUEST['copiar'] ) {
			case '':
				# quando usu�rio quer copiar a atividade e suas filhas
				array_push( $origem, $_REQUEST['origem'] );
				break;
			case '':
				# quando o usu�rio quer copiar as filhas apenas
				$sql = "select atiid from pde.atividade where atiidpai = " . $_REQUEST['origem'];
				foreach ( (array) $db->carregar( $sql ) as $atividade ) {
					if ( !$atividade ) {
						continue;
					}
					array_push( $origem, (integer) $atividade['atiid'] );
				}
				break;
			default:
				break;
		}
		
		switch ( $_REQUEST['colar'] ) {
			case '':
				if ( !atividade_verificar_responsabilidade( $_REQUEST['destino'], $_REQUEST['usucpf'] ) ) {
					echo 'Usu�rio sem permiss�o para alterar a atividade.';
					exit();
				}
				# quando o usu�rio quer colar dentro de uma atividade
				break;
			case '':
				$atiid = $db->pegaUm( "select atiidpai = from pde.atividade where atiid = ". (integer) $_REQUEST['destino'] );
				if ( !atividade_verificar_responsabilidade( $atiid, $_REQUEST['usucpf'] ) ) {
					echo 'Usu�rio sem permiss�o para alterar a atividade.';
					exit();
				}
				# quando o usu�rio quer colar ap�s uma atividade
				break;
			default:
				break;
		}
		exit();

	default:
		echo '';
		exit();

}

?>
