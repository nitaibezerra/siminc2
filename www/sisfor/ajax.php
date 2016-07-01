<?php

/**
 * Centraliza as requisições ajax do módulo.  
 *
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007 
 */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

// carrega as funções do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

if($_REQUEST['atualizarFormulario']){
    ob_clean();
    montarFormularioEquipe($_REQUEST['pflcod'], $_REQUEST['tpeid']);
    die;
}

if($_REQUEST['atualizarFormularioCursista']){
    ob_clean();
    montarFormularioCursista($_REQUEST['sifid'], $_REQUEST['fpbid']);
    die;
}

if($_REQUEST['deletaTipoPerfil']){
    ob_clean();

    if($_REQUEST['tpeid']){
    	
    	$pboid = $db->pegaUm("SELECT pboid FROM sisfor.pagamentobolsista WHERE tpeid='".$_REQUEST['tpeid']."'");
    	
    	$avaliacoes_autorizadas = $db->pegaUm("SELECT count(*) as n FROM sisfor.folhapagamentoprojeto fp 
												INNER JOIN workflow.documento d ON d.docid = fp.docid 
												INNER JOIN sisfor.tipoperfil t ON t.sifid = fp.sifid 
												INNER JOIN sisfor.mensario m ON m.tpeid = t.tpeid 
    											INNER JOIN sisfor.mensarioavaliacoes ma ON ma.menid = m.menid
												WHERE m.tpeid={$_REQUEST['tpeid']} AND d.esdid=".ESD_ENVIADO_PAGAMENTO." AND ma.mavatividadesrealizadas='A'");
    	
    	if($pboid || $avaliacoes_autorizadas) {
    		echo "<script>alert('Usuário possui pagamento e/ou avaliações autorizadas para pagamento, por isso não pode ser removido.');</script>";
    	} else {
    		$sql = "delete from sisfor.mensario where tpeid = '{$_REQUEST['tpeid']}'";
    		$db->executar($sql);
	        $sql = "delete from sisfor.tipoperfil where tpeid = '{$_REQUEST['tpeid']}'";
	        $db->executar($sql);
	        $db->commit();
    	}
    }

    montarFormularioEquipe($_REQUEST['pflcod']);
    die;
}

if($_REQUEST['recuperarUsuario']){
    ob_clean();
    global $db;
    $cpf = ereg_replace( "[^0-9]", "", $_REQUEST['iuscpf']);

    $aRetorno = array('cpf'=>null, 'nome'=>null, 'email'=>null);
    if($cpf){

        $dados = recuperarUsuarioReceita($cpf);
        if($dados['usuarioexiste']){
            $aRetorno['cpf'] = $cpf;
            $aRetorno['nome'] = $dados['dados']['no_pessoa_rf'];

            // Recupera primeiramente os dados de sisfor.identificacaousuario
            $sql = "select iusd, iuscpf, iusnome, iusemailprincipal from sisfor.identificacaousuario where iuscpf = '{$cpf}'";
            $dados = $db->pegaLinha($sql);
            // Se houver os dados do cpf informado, recupera
            if(is_array($dados) && count($dados)){
                $aRetorno['email'] = $dados['iusemailprincipal'];
            } else {
                $sql = "select * from seguranca.usuario where usucpf = '{$cpf}'";
                $dados = $db->pegaLinha($sql);

                // Se houver os dados do cpf informado, recupera
                if(is_array($dados) && count($dados)){
                    $aRetorno['email'] = $dados['usuemail'];
                }
            }
        }
    }
    echo simec_json_encode($aRetorno);

    die;
}

if($_REQUEST['verificaUsuarioDuplicado']){
    ob_clean();
    global $db;
    $cpf    = ereg_replace( "[^0-9]", "", $_REQUEST['iuscpf']);
    $pflcod = $_REQUEST['pflcod'];

    $aRetorno = array('permitido'=>true, 'mensagem'=>'');
    if($cpf){
        $sql = "select count(*)
                from sisfor.identificacaousuario u
                        inner join sisfor.tipoperfil t on t.iusd = u.iusd
                where t.sifid = '{$_SESSION['sisfor']['sifid']}'
                and u.iuscpf = '$cpf' and t.pflcod = '$pflcod'";
        $existeCPF = $db->pegaUm($sql);

        if($existeCPF){
            $aRetorno = array('permitido'=>false, 'mensagem'=>utf8_encode('CPF já vincvulado.'));
        }
    }
    echo simec_json_encode($aRetorno);

    die;
}
