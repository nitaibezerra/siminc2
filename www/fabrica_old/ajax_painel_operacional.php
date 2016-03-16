<?php
header( 'Content-Type: text/html; charset=iso-8859-1' );

//Carrega parametros iniciais do simec
include_once "config.inc";

include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . 'www/fabrica/_constantes.php';
require_once APPRAIZ . 'www/fabrica/_componentes.php';
require_once APPRAIZ . 'www/fabrica/_funcoes.php';

require_once APPRAIZ . 'fabrica/classes/PainelOperacional.php';


// Cria instância do banco
$db                 = new cls_banco();
$painelOperacional  = new PainelOperacional( $db );

if ($_REQUEST['celidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
        $celid  = (int) $_REQUEST['celidAjax'];
        $sidid = (int) $_REQUEST['sididAjax'];
 
	$sql = sprintf("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = %d
					order by s.sidabrev", 
			$celid);
	$db->monta_combo( 'sidid', $sql, 'S', '-- Informe o Sistema --', '', '','','','','sidid','','','',$sidid);
	exit;
}


if ( isset( $_REQUEST['action'] ) ) {
    
    $nomeEstado = '';

    try {

        $esdId      = (int) $_REQUEST['esdid'];
        
        $sqlNomeEstado = "SELECT tpddsc ||' - '|| esddsc as nomeEstado
                            FROM workflow.estadodocumento esd
                            INNER JOIN workflow.tipodocumento tpd
                                ON esd.tpdid = tpd.tpdid
                            WHERE esdid = {$esdId}";
        
        $nomeEstado = $db->pegaUm( $sqlNomeEstado );

        switch ( $_REQUEST['action'] ) {
            case 'listarSolicitacaoServico':
                $listagem = $painelOperacional->listarSolicitacaoServicoPorEstado( $esdId );
                break;
            case 'listarOrdemServico':
                $tosId    = (int) $_REQUEST['tosid'];
                $listagem = $painelOperacional->listarOrdemServicoPorEstadoTipo( $esdId, $tosId );
                break;
            
             case 'painelgProjetos':
                 
                    $esdId      = (int) $_REQUEST['esdid'];
                    $sidid      = $_REQUEST['sidid'];
                    $celid      = $_REQUEST['celid'];
                    //exit("aqui: " . WF_ESTADO_OS_APROVACAO);
                    $dados      = array("esdId" => $esdId, "sidid" => $sidid , "celid" => $celid);
                    
                    switch ( $esdId ) {
                        case WF_ESTADO_PRE_ANALISE:
                             $nomeEstado = "Realizar Pré-Análise";
                             break;    
                        case WF_ESTADO_APROVACAO:
                             $nomeEstado = "Aprovar Execução do Serviço";
                              break;
                        case WF_ESTADO_OS_APROVACAO: 
                            $nomeEstado = "Homologar Ordem de Serviço";
                             break;
                        case WF_ESTADO_DETALHAMENTO:
                             $nomeEstado = "OS Em Detalhamento";
                             break;
                        case 0000:
                             $nomeEstado = "OS Em Execução";
                             break;
                        case WF_ESTADO_OS_PAUSA:  
                            $nomeEstado = "OS Em Pausa"; 
                             break;
                    }
                    //echo "";
                    //    var_dump($dados);
                     //       exit;
                    $listagem = $painelOperacional->painelGerenteProjetos( $dados );
                    
                break;
            
            default;
                echo 'Ação não disponível';
        }

    } catch ( Exception $e ) {
        $listagem = $e->getMessage();
    }

    echo'<table width="100%" cellspacing="1" cellpadding="0">
            <tr>
                <td valign="top" class="TituloTabela center" id="nomeListagem" >' . $nomeEstado . '</td>
            </tr>
        </table>' . $listagem ;

/*
    echo simec_json_encode( array(
        'nomeEstado' => utf8_encode( $nomeEstado ),
        'listagem'   => utf8_encode( $listagem )
    ) );
*/
    exit;
}



if ( isset( $_REQUEST['listarPainelCelulaSistema'] ) ) {
   
    
//    echo "<script type=\"text/javascript\" src=\"./js/jquery-1.7.min.js\"></script>
//<script type=\"text/javascript\" src=\"./js/painel-operacional-gp.js\"></script>";
    
    $celid     = $_REQUEST['celid'];
    $sidid     = $_REQUEST['sidid'];   
     
    $dados = array("celid" => $celid, "sidid" => $sidid);
    
    $listagem =  $painelOperacional->painelOperacionalGerenteProjetos($dados);
    
    echo $listagem ;
    
    echo "<script type=\"text/javascript\">
            $('.painelGerenteProjetosSS').click( PainelOperacionalGerenteProjetoView.painelGerenteProjetosSSClickHandler );
          </script>";
    
    
    exit;
}