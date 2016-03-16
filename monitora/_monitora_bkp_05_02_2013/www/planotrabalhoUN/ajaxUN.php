<?php

/**
 * Centraliza as requisições ajax do módulo.  
 *
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br> 
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

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções de integração
require_once( APPRAIZ."monitora/www/planotrabalhoUN/unidade_atividade_funcoes.php" );

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// carrega as funções do módulo pde
include APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
include APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";
include APPRAIZ . "monitora/www/planotrabalhoUN/_componentes.php";

switch ( $_REQUEST['evento'] ) {

	case 'arvore_alterar_atividade':
		// verifica permissão
//		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para alterar a atividade.';
			exit();
		}
		if ( in_array( $_REQUEST['campo'], array( 'atidatainicio', 'atidatafim', 'atidataconclusao' ) ) ) {
			switch( $_REQUEST[ 'campo' ] ) {
				case 'atidatainicio':
					// a nova data de inicio nao pode ser posterior a data de termino
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] ,  $_REQUEST['valor'] , null, null ) ) {
						echo " A data de início não pode ser posterior a data de término";
						exit();
					}
					break;
				case 'atidatafim':
					// a nova data de termino nao pode ser anterior a data de inicio
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , $_REQUEST['valor'], null ) ) {
						echo " A data de término não pode ser anterior a data de início";
						exit();
					}
					break;
				case 'atidataconclusao':
					// a nova data de conclusao nao pode ser anterior a data de inico
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , null, $_REQUEST['valor'] ) ) {
						echo " A data de conclusão não pode ser anterior a data de início";
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
		
		if($_REQUEST[ 'campo' ] == 'atidatainicio' || $_REQUEST[ 'campo' ] == 'atidatafim' || $_REQUEST[ 'campo' ] == 'atidataconclusao'){
			$valor = formata_data_sql($valor);
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
		// verifica permissão
		//if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para excluir a atividade.';
			exit();
		}
		// efetiva a exclusão
		if( !atividade_excluir( $_REQUEST['atiid'] ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da árvore
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_inserir':
		$subacao = array("subatv"    => $_REQUEST['subatv'],
						 "sbatitulo" => $_REQUEST['sbatitulo'],
						 "sbacod"    => $_REQUEST['sbacod'],
						 "sbadsc"    => $_REQUEST['sbadsc']);
		
		// verifica permissão no pai do item novo
//		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiidpai'], $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para cadastrar atividade.';
			exit();
		}
		
		// verifica se o título foi preenchido
		$titulo = trim( $_REQUEST['atidescricao'] );
		if(!$subacao['subatv']){
			if ( empty( $titulo ) ){
				echo 'O título é obrigatório.';
				exit();
			}			
		}
		
		// efetiva a inserção
		if ( !atividade_inserir( $_REQUEST['atiidpai'], $titulo, $subacao ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza projeto da sessão
		$sql = sprintf( "select _atiprojeto from pde.atividade where atiid = %d", $_REQUEST['atiidpai'] );
		$_SESSION['projeto'] = (integer) $db->pegaUm( $sql );
		// atualiza dados da árvore
		atividade_calcular_dados( $_REQUEST['atiidpai'] );
		$db->commit();
		
		if($subacao['subatv']) {
			die("<script>
					window.opener.location.replace(window.opener.location);
					alert('Subação inserida com sucesso');
					window.close();
				 </script>");
		}
		exit;

	case 'arvore_mudar_ordem':
		// verifica permissão no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = ". (integer) $_REQUEST['origem'];
		$atiidpai = $db->pegaUm( $sql );
//		if ( !atividade_verificar_responsabilidade( $atiidpai, $_REQUEST['usucpf'] ) ) {
		if(false) {
			echo 'Usuário sem permissão para alterar a atividade.';
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
		
		// atualiza dados da árvore
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_mudar_nivel':
		// verifica permissão
//		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para alterar a atividade.';
			exit();
		}
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai_antigo = $db->pegaUm( $sql );
		// efetiva a mudança de nível
		$funcao = $_REQUEST['direcao'] == 'esquerda' ? 'atividade_profundidade_esquerda' : 'atividade_profundidade_direita';
		if ( !$funcao( $_REQUEST['atiid'] ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da árvore
		$sql = "select atiidpai from pde.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai_novo = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai_antigo );
		atividade_calcular_dados( $atiidpai_novo );
		$db->commit();
		exit();

	case 'arvore_recarregar':
		// carrega a árvore
		$lista = atividade_listar( $_REQUEST['atividade'], $_REQUEST['profundidade'], $_REQUEST['situacao'],  $_REQUEST['usuario'] );
		$diretorio = $_SESSION["sisdiretorio"] . "/atividade";
		//ver($lista,d);
		echo arvore_corpo( $lista, $diretorio );
		exit();

	case 'arvore_mudar_pai':
		// verifica permissão no pai do item a ser movido
//		if ( !atividade_verificar_responsabilidade( $_REQUEST['pai'], $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		$avo = $atividade["atiidpai"];
		// move para trás os antigos irmãos
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
		// verifica permissão no pai do item a ser movido
		$sql = "select atiidpai from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$pai = $db->pegaUm( $sql );
		//if ( !atividade_verificar_responsabilidade( $pai, $_REQUEST['usucpf'] ) ) {
		if ( false ) {
			echo 'Usuário sem permissão para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		// move para trás os antigos irmãos
		$sql = "update pde.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// captura os dados do novo irmão
		$sql = "select atiid, atiidpai, atiordem from pde.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$irma = $db->pegaLinha( $sql );
		// move os novos irmãos pra frente
		$sql = "update pde.atividade set atiordem = atiordem + 1 where atiidpai = " . (integer) $irma["atiidpai"] . " and atiordem > " . (integer) $irma["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// coloca a atividade na nova posição
		$sql = "update pde.atividade set atiordem = " . ( $irma["atiordem"] + 1 ) . ", atiidpai = ". $irma["atiidpai"] ." where atiid = " . $_REQUEST["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// organiza a árvore
		atividade_calcular_dados( $pai );
		$db->commit();
		exit();
		
	default:
		echo '';
		exit();

}

?>