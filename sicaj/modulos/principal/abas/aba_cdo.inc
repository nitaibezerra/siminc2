<div class="row col-md-11">
    <div class="row">
        <div class="col-md-12">&nbsp;</div>
    </div>
    <div class="row">
        <form name="registroFile" id="registroFile" method="POST" enctype="multipart/form-data" role="form">
            <div class="well">
                <div class="form-horizontal">
                    <div class="form-group">
                        <div class="col-md-3">
                            <label class="control-label" for="pdcvalorexecatual">Valor no Exercício Atual (R$):</label>
                        </div>
                        <div class="col-md-9">
                            <p class="form-control-static" id="pdcvalorexecatual"><?= number_format($dados['pdcvalorexecatual'], 2, ',', '.')?></p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-md-3">
                            <label class="control-label" for="pdcvalorexecpassado">Valor no Exercício Anterior (R$):</label>
                        </div>
                        <div class="col-md-9">
                            <p class="form-control-static" id="pdcvalorexecpassado"><?= number_format($dados['pdcvalorexecpassado'], 2, ',', '.')?></p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-md-3">
                            <label class="control-label" for="pdcvalorcdo">Valor do CDO (R$):</label>
                        </div>
                        <div class="col-md-9">
                            <strong class="cdo_total" data-thousands="." data-decimal="," data-prefix="">
                                <?php echo number_format($dados['total_cdo'], 2, ',', '.'); ?>
                            </strong>
                        </div>
                    </div>
                </div>
            </div>
            <br style="clear:both" />
            <div class="col-md-2">
                <div class="form-group row alert">
                    <input type="file"
                           class="btn btn-info start"
                           name="outrosDocumentos"
                           id="outrosDocumentos"
                           title="Outros documentos">
                </div>
                <br />
                <?php if (!existeArquivoTipo(HOMOLOGACAO_SICAJ, $_GET['id'])) : ?>
                <div class="form-group row alert">
                    <input type="file"
                           class="btn btn-danger start"
                           name="homologacaoSICAJ"
                           id="homologacaoSICAJ"
                           title="Homologação SICAJ">
                </div>
                <br />
                <?php endif; ?>

                <?php if (!in_array(PERFIL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))): ?>

                <button type="reset" class="btn btn-warning" id="limpar">Limpar</button>
                <button type="button" class="btn btn-primary" id="inserir">Salvar</button>
                <?php else: gravacaoDesabilitada('', 12, 0); endif; ?>

                <?php if (HOMOLOGADO == pegarEstadoAtual($_GET['id'])) : ?>
                    <br /><br />
                    <a href="sicaj.php?modulo=principal/impressao&acao=A&id=<?=$_GET['id'];?>"
                       target="_blank"
                       class="btn btn-primary"
                       id="print">Impressão do CDO</a>
                <?php endif; ?>
            </div>
        </form>
        <div class="col-md-10">
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
                <tr>
                    <?= imprimeTabelaAnexos('O2', $_GET['id']) ?>
                </tr>
                </tbody>
            </table>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th width="5%"></th>
                    <th width="25%" class="text-left">Homologação SICAJ</th>
                    <th width="15%" class="text-left">Tamanho(bytes)</th>
                    <th width="15%" class="text-left">Data inclusão</th>
                    <th width="15%" class="text-left">Responsável</th>
                </tr>
                </thead>
                <tbody>
                <tr>
                    <?= imprimeTabelaAnexos('HS', $_GET['id']) ?>
                </tr>
                </tbody>
            </table>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th width="5%"></th>
                    <th width="25%" class="text-left">Mensagem eletrônica da SEGEP</th>
                    <th width="15%" class="text-left">Tamanho(bytes)</th>
                    <th width="15%" class="text-left">Data inclusão</th>
                    <th width="15%" class="text-left">Responsável</th>
                </tr>
                </thead>
                <tbody>
                <tr>
                    <?= imprimeTabelaAnexos('MS', $_GET['id']) ?>
                </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>
<div class="row col-md-1">
    <?php require dirname(__FILE__) . '/../tramitacao.inc';  ?>
</div>
<br style="clear:both" />
<br />
<br />
<script type="text/javascript">
;(function(){
    var Cdo;
    Cdo = window.Cdo = {};

    Cdo.Boot = function(dom) {
        this.DOM = $(dom);

        this.exercicio = new Cdo.Exercicio(
            this.DOM.find("#pdcvalorexecatual")
          , this.DOM.find("#pdcvalorexecpassado")
          , this.DOM.find(".cdo_total")
        );
    };
})();

;(function(Cdo){
    Cdo.Exercicio = function(fieldA, fieldB, container) {
        this.pdcvalorexecatual = fieldA;
        this.pdcvalorexecpassado = fieldB;
        this.cdo_total = container;
        this.eventListeners();
    };

    Cdo.Exercicio.prototype.eventListeners = function() {
        this.pdcvalorexecpassado.on("blur", $.proxy(this, "isValid"));
    };

    Cdo.Exercicio.prototype.isValid = function() {
        if (!this.pdcvalorexecatual.val() || !this.pdcvalorexecpassado.val()) {
            this.cdo_total.html(this.pdcvalorexecatual.val() || this.pdcvalorexecpassado.val());
            return false;
        }

        this.somaCdo();
    };

    Cdo.Exercicio.prototype.somaCdo = function() {
        var total, atual, passado, $requet, that = this;

        atual = this.pdcvalorexecatual.val().replace(/\./g, '').replace(',', '.');
        passado = this.pdcvalorexecpassado.val().replace(/\./g, '').replace(',', '.');

        $requet = $.ajax({url:location.href+"&currency=true&atual="+atual+"&passado="+passado});
        $requet.done(function(data) {
            that.cdo_total.html(data);
        });
    };
})(Cdo);

$(function(){
    $("._delete_").on("click", function(e){
        e.preventDefault();
        var valorRef = ($(this).attr("href").split("#")[1]);
        if (valorRef) {
            location.href="sicaj.php?modulo=principal/principal&acao=A&unicod=<?=$_GET['unicod'];?>&id=<?=$_GET['id'];?>&aba=cdo&deleteFile="+valorRef;
        }
    });

    $("._download_").on("click", function(e){
        e.preventDefault();
        var valorRef = ($(this).attr("href").split("#")[1]);
        if (valorRef) {
            location.href="sicaj.php?modulo=principal/principal&acao=A&unicod=<?=$_GET['unicod'];?>&id=<?=$_GET['id'];?>&aba=cdo&downloadFile="+valorRef;
        }
    });

    $("#limpar").on("click", function(){
        location.href=location.href;
    });

    /**
     * Ação de inserir para o formulário de CDO
     */
    $("#inserir").livequery("click", function(){
        errors = false;

        if (errors) return false;
        $(this).attr("disabled", true);
        $("#registroFile").submit();
    });

    Cdo.Boot(document.body);
});
</script>
