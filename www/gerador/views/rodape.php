<hr>
<div class="alert alert-info">
    <h4>Dados da Conexão Atual</h4>
    <table class="table table-hover table-striped" style="width: 50%">
        <tr>
            <td><b>Servidor:</b></td>
            <td><?= $GLOBALS['servidor_bd'] ?></td>
            <td><b>Porta:</b></td>
            <td><?=$GLOBALS['porta_bd']?></td>
        </tr>
        <tr>
            <td><b>Database:</b></td>
            <td><?= $GLOBALS['nome_bd'] ?></td>
            <td><b>Usuário:</b></td>
            <td><?= $GLOBALS['usuario_db'] ?></td>
        </tr>
    </table>
</div>
<script type="text/javascript">
    $(function(){
        $('select').chosen();
    });
</script>
