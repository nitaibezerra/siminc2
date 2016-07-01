<? 
function redirecionar( $modulo, $acao, $parametros = array() )
{
    $parametros = http_build_query( (array) $parametros, '', '&' );
    header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
    exit();
}

switch( $_REQUEST['evento'] ){
    case 'excluir_anexo':
        $sql     ="SELECT * FROM monitora.documentos WHERE docstatus = 'A' AND docid = ".$_REQUEST['docid'];
        $arquivo = $db->pegaLinha($sql);

        $sql = sprintf( "UPDATE monitora.documentos SET docstatus = 'E' WHERE docid = %d", $_REQUEST['docid'] );
        if( !$db->executar( $sql ) ){
            $_SESSION['MSG_AVISO'][] = "Não foi possível remover o documento.";
            $db->rollback();
        } else {
            // enquanto nao pode ser removido de verdade... fica comentáda a linha
            //if(unlink($this->arquivo_caminho))
            //{

            $db->commit();

            if (is_file(APPRAIZ . 'arquivos/monitora/'.$arquivo['doccaminho'])) {
                if (unlink(APPRAIZ . 'arquivos/monitora/'.$arquivo['doccaminho'])) {
                    $_SESSION['MSG_AVISO'][] = "Documento excluído com êxito";
                } else {
                    $_SESSION['MSG_AVISO'][] = "Não foi possível remover o documento.";
                }
            } else {
                $_SESSION['MSG_AVISO'][] = "Não foi possível remover o arquivo. Arquivo inexistente.";
            }
        }

        redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
        break;

    case 'download':
        $sql ="SELECT * FROM monitora.documentos WHERE docstatus = 'A' AND docid = ".$_REQUEST['arquivo'];
        $arquivo = $db->pegaLinha( $sql );
        $caminho = APPRAIZ . 'arquivos/monitora/'.$arquivo['doccaminho'];

        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
		ob_clean();
        header( 'Content-type: '. $arquivo['docmimetype'] );
        header( 'Content-Disposition: attachment; filename='.$arquivo['docnome']);
        readfile( $caminho );
        exit();
        break;

    default:
        break;

}

//recupera todas as variaveis que veio pelo post ou get

foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
monta_titulo('Coordenação-Geral de Planejamento - SPO', 'Notas Técnicas Para Download');
?>
<script language="javascript" type="text/javascript">    
    function excluir_anexo( anexo ){
        if ( confirm( 'Deseja excluir o documento?' ) ) {
            window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir_anexo&docid='+ anexo;
        }
    }
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
    <tr>
        <td>
        <!-- LISTA DE DOCUMENTOS -->
    <?php
        $sql ="SELECT 
              doc.docid,
              doc.doctamanho,
              doc.docnome,
              doc.docdescricao,
              to_char( doc.docdata,'dd/mm/YYYY') as docdata,
              usu.usunome,
              ta.taadescricao
        FROM monitora.documentos AS doc
            INNER JOIN seguranca.usuario 	  AS usu ON (usu.usucpf = doc.usucpf)
            INNER JOIN pde.tipoanexoatividade AS ta  ON (ta.taaid   = doc.taaid)
        WHERE docstatus = 'A' 
        ORDER BY doc.docdata DESC
        ";
        $documentos = $db->carregar( $sql );
        if( is_array( $documentos ) ): ?>
        <table class='tabela' style="width:100%;" cellpadding="3">
            <thead>
                <tr style="text-align: center; vertical-align: top">
                    <th style="width: 5%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Ação	
                    </td>				
                
                    <th style="width: 20%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Arquivo
                    </th>
                    <th style="width: 5%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Tamanho
                    </th>
                    <th style="width: 35%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Descrição
                    </th>
                    <th style="width: 10%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Tipo
                    </th>
                    <th style="width: 10%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Data de Inclusão
                    </th>
                    <th style="width: 20%; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                        Por
                    </th>
                </tr>
            </thead>				
            
            <tbody>
            <?php
            /*
            echo "<pre>";
            print_r($documentos);
            Array
            (
                [0] => Array
                (
                    [docid] => 1
                    [taaid] => 1
                    [usucpf] => 
                    [doctamanho] => 1
                    [docnome] => nota_concurso_cargos_mec_rfet.doc
                    [docdescricao] => teste de nao sei o que lah das quantas....
                    [docdata] => 2008-01-24 17:53:59.074084
                )

            )
            */
            foreach( $documentos as $documento ): ?>
                <?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; 
                /*
                    echo "<pre>";
                    print_r($documento);
                    */
                ?>
                
                <tr style=" background-color: <?= $cor ?>" >
                    <td style="text-align:center" >
                        <?php if( $db->testa_superuser() || $db->testa_cgo() ): ?>
                            <img title="Remover o Documento" align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluir_anexo( <?= $documento['docid'] ?> );" title=""/>
                        <?php else: ?>
                            &nbsp;
                        <?php endif; ?>
                    </td>				
                    <td>
                    <!-- <img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/> -->
                        <a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=download&arquivo=<?=$documento['docid']?>">
                            <?= $documento['docnome'] ?>
                        </a>
                    </td>
                    <td style="text-align: center"><?= round($documento['doctamanho']/1024)." Kb" ?></td>
                    <td style="text-align: left"><?= $documento['docdescricao'] ?></td>
                    <td style="text-align: center"><?= $documento['taadescricao'] ?></td>
                    <td style="text-align: center"><?= $documento['docdata'] ?></td>
                    <td nowrap="nowrap"><?= $documento['usunome'] ?></td>				
            <?php endforeach; ?>
            </tbody>
        </table>
        <?php else: ?>
            <table class='tabela' style="width:100%;" cellpadding="3">
                <tbody>
                    <td style="text-align:center;padding:15px;background-color:#f5f5f5;">
                        Nenhum Documento Encontrado.
                    </td>
                </tbody>
            </table>
        <?php endif; ?>
            
        </td>
    </tr>
</table>