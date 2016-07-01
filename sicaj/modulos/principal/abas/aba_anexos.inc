<?php $btnclass = ('H' == $dados['pdctipo'])?'btn-danger':'btn-info'; ?>
<div class="row col-md-11">
    <div class="row">
        <div class="col-md-12">&nbsp;</div>
    </div>
    <div class="row">
        <div class="col-md-2">
            <form name="registroFile" id="registroFile" method="POST" enctype="multipart/form-data" role="form">
                <?php if (!existeArquivoTipo(DECISAO_JUDICIAL, $_GET['id'])) : ?>
                <div class="form-group row alert">
                    <input type="file"
                           class="btn <?php echo $btnclass; ?> start"
                           name="cpDecisaoJudicial"
                           id="cpDecisaoJudicial"
                           title="Decisão judicial">
                </div>
                <br />
                <?php endif; ?>
                <?php if (!existeArquivoTipo(PARECER_EXECUTORIO, $_GET['id'])) : ?>
                <div class="form-group row alert">
                    <input type="file"
                           class="btn <?php echo $btnclass; ?> start"
                           name="cpParecerExecutorio"
                           id="cpParecerExecutorio"
                           title="Parecer de força executória">
                </div>
                <br />
                <?php endif; ?>
                <?php if (!existeArquivoTipo(PLANILHA_FINANCEIRA, $_GET['id'])) : ?>
                <div class="form-group row alert">
                    <input type="file"
                           class="btn <?php echo $btnclass; ?> start"
                           name="cpPlanilhaFinanceira"
                           id="cpPlanilhaFinanceira"
                           title="Planilha financeira*">
                </div>
                <br />
                <?php endif; ?>

                <div class="form-group row alert">
                    <input type="file"
                           class="btn btn-info start"
                           name="outrosDocumentos"
                           id="outrosDocumentos"
                           title="Outros documentos">
                </div>
                <br />

                <button type="reset" class="btn btn-warning" id="limpar">Limpar</button>
                <button type="button" class="btn btn-primary" id="inserir">Salvar</button>
            </form>
            <script type="text/javascript">
            $(function(){
                /**
                 * Ação de inserir para o formulário de anexos
                 */
                $("#inserir").on("click", function(){
                    $(this).attr("disabled", true);
                    $("#registroFile").submit();
                });
            });
            </script>
        </div>
        <div class="col-md-10">
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th width="5%"></th>
                        <th width="25%" class="text-left">Cópia da decisão judicial</th>
                        <th width="15%" class="text-left">Tamanho(bytes)</th>
                        <th width="15%" class="text-left">Data inclusão</th>
                        <th width="15%" class="text-left">Responsável</th>
                    </tr>
                </thead>
                <tbody>
                    <?= imprimeTabelaAnexos('DJ', $_GET['id']) ?>
                </tbody>
            </table>

            <table class="table table-striped">
                <thead>
                    <tr>
                        <th width="5%"></th>
                        <th width="25%" class="text-left">Cópia do parecer de força executória</th>
                        <th width="15%" class="text-left">Tamanho(bytes)</th>
                        <th width="15%" class="text-left">Data inclusão</th>
                        <th width="15%" class="text-left">Responsável</th>
                    </tr>
                </thead>
                <tbody>
                    <?= imprimeTabelaAnexos('PE', $_GET['id']) ?>
                </tbody>
            </table>

            <table class="table table-striped">
                <thead>
                    <tr>
                        <th width="5%"></th>
                        <th width="25%" class="text-left">Cópia da planilha financeira</th>
                        <th width="15%" class="text-left">Tamanho(bytes)</th>
                        <th width="15%" class="text-left">Data inclusão</th>
                        <th width="15%" class="text-left">Responsável</th>
                    </tr>
                </thead>
                <tbody>
                    <?= imprimeTabelaAnexos('PF', $_GET['id']) ?>
                </tbody>
            </table>

            <table class="table table-striped">
                <thead>
                    <tr>
                        <th width="5%"></th>
                        <th width="25%" class="text-left">Outros documentos</th>
                        <th width="15%" class="text-left">Tamanho(bytes)</th>
                        <th width="15%" class="text-left">Data inclusão</th>
                        <th width="15%" class="text-left">Responsável</th>
                    </tr>
                </thead>
                <tbody>
                    <?= imprimeTabelaAnexos('OD', $_GET['id']) ?>
                </tbody>
            </table>
        </div>
    </div>
</div>
<div class="row col-md-1">
    <?php require dirname(__FILE__) . '/../tramitacao.inc';  ?>
</div>

<script type="text/javascript">
$(function(){
    $("._delete_").on("click", function(e){
        e.preventDefault();
        var valorRef = ($(this).attr("href").split("#")[1]);
        bootbox.confirm('Deseja apagar o arquivo selecionado?', function(result){
            if (result && valorRef) {
               location.href="sicaj.php?modulo=principal/principal&acao=A&unicod=<?=$_GET['unicod'];?>&id=<?=$_GET['id'];?>&aba=anexos&deleteFile="+valorRef;
            }
        });
    });

    $("._download_").on("click", function(e){
        e.preventDefault();
        var valorRef = ($(this).attr("href").split("#")[1]);
        if (valorRef) {
            location.href="sicaj.php?modulo=principal/principal&acao=A&unicod=<?=$_GET['unicod'];?>&id=<?=$_GET['id'];?>&aba=anexos&downloadFile="+valorRef;
        }
    });

    $("#limpar").on("click", function(){
        location.href=location.href;
    });
});
</script>