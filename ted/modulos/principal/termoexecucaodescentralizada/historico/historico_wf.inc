<?php

include APPRAIZ . 'includes/workflow.php';

$docid = (integer) Ted_Utils_Model::getDocid(Ted_Utils_Model::capturaTcpid());
$documento = wf_pegarDocumento($docid);
$atual = wf_pegarEstadoAtual($docid);
$historico = wf_pegarHistorico($docid);

?>
<script type="text/javascript">
IE = !!document.all;
function exebirOcultarComentario(docid) {
    id = 'comentario' + docid;
    div = document.getElementById( id );
    if ( !div )
    {
        return;
    }
    var display = div.style.display != 'none' ? 'none' : 'table-row';
    if ( display == 'table-row' && IE == true )
    {
        display = 'block';
    }
    div.style.display = display;
}
</script>

<form action="" method="post" name="formulario" class="form-horizontal" role="form">
    <table class="table table-bordered table-striped table-hover">
        <thead>
            <tr>
                <th colspan="6">
                    <h3>Histórico de Tramitações</h3>
                    <h5>Termo de Execução Descentralizada</h5>
                </th>
            </tr>
            <?php if ( count( $historico ) ) : ?>
                <tr>
                    <th style="width: 20px;">Seq.</th>
                    <th style="width: 200px;">Onde Estava</th>
                    <th style="width: 200px;">O que aconteceu</th>
                    <th style="width: 90px;">Quem fez</th>
                    <th style="width: 120px;">Quando fez</th>
                    <th style="width: 17px;">&nbsp;</th>
                </tr>
            <?php endif; ?>
        </thead>
        <?php $i = 1; ?>
        <?php foreach ( $historico as $item ) : ?>
            <?php //$marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
            <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
                <td class="text-center"><?=$i?>.</td>
                <td class="text-left" style="color:#008000;">
                    <?php echo $item['esddsc']; ?>
                </td>
                <td class="text-left">
                    <?php echo $item['aeddscrealizada']; ?>
                </td>
                <td  class="text-left" style="font-size: 10px;">
                    <?php echo $item['usunome']; ?>
                </td>
                <td class="text-center">
                    <?php echo $item['htddata']; ?>
                </td>
                <td class="text-center">
                    <?php if ($item['cmddsc']) : ?>
                        <img
                            align="middle"
                            style="cursor: pointer;"
                            src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/restricao.png"
                            onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
                            />
                    <?php endif; ?>
                </td>
            </tr>
            <tr id="comentario<?php echo $i; ?>" style="display: none;">
                <td colspan="6">
                    <div >
                        <?php echo simec_htmlentities( $item['cmddsc'] ); ?>
                    </div>
                </td>
            </tr>
            <?php $i++; ?>
        <?php endforeach; ?>
        <?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
        <tr>
            <td class="text-right" colspan="6">
                Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
            </td>
        </tr>
    </table>
</form>