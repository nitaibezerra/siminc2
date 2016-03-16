<?php
$notaCredito = new Ted_Model_NotaCredito();
$dadosNC = $notaCredito->getByTcpid(Ted_Utils_Model::capturaTcpid());
$jsonDadosNC = json_encode(array('nc' => $dadosNC));
//ver($jsonDadosNC);
?>
<script id="table-notacredito" type="text/x-handlebars-template">
    <tr>
        <td>
            <span class="glyphicon glyphicon-remove cursor-click remove-nc newLine" data-action="notacredito" data-remove-id="{{ppaid}}"></span>
            <input type="hidden" value="{{ppaid}}" name="ppaid[]" id="ppaid-{{ppaid}}" />
        </td>
        <td class="text-center" width="">
            {{notacredito}}
        </td>
        <td class="text-center" width="">
            {{numtransfsiafi}}
        </td>
        <td class="text-center" width="">
            <input type="text" class="form-control text-right" name="vlrparcela[]" id="parcela-{{ppaid}}" value="{{vlrparcela}}" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
        </td>
        <td>
            {{ppadata}}
        </td>
    </tr>
</script>
<script type="text/javascript">
    $(function() {
        $("#addNC").on("click", function() {
            var templateString = Handlebars.compile($("#table-notacredito").html());
            actionNotaCredito(templateString);
        });

        loadGridNC();
        $("[name='vlrparcela[]']").focus().blur();
    });

    /**
     * Carga de dados para grid de Nota Credito [Programação orçamentária]
     * terceira parte
     * @return void(0)
     */
    var loadGridNC = function() {
        var jsonStringfy = <?=$jsonDadosNC;?>;
        if (typeof jsonStringfy === 'object') {
            if (jsonStringfy.nc) {
                var template = Handlebars.compile($("#table-notacredito").html());
                jsonStringfy.nc.forEach(function(objData, i){
                    //console.log(objData);
                    actionNotaCredito(template, objData);
                });
            }
        }
    };

    var actionNotaCredito = function(template, objData) {
        var html
          , objData = objData || {};

        objData["ppaid"] = objData.ppaid || new Date().getTime();
        html = template(objData);
        if ($("#tb_notacredito tbody tr").length) {
            $("#tb_notacredito tbody tr").last().after(html);
        } else {
            $("#tb_notacredito tbody").append(html);
        }
    };
</script>
<div class="row col-md-12">
    <button class="btn btn-primary btn-sm" id="addNC">
        <span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
        &nbsp;Adicionar
    </button>

    <table class="table table-bordered tb_render" id="tb_notacredito">
        <thead>
        <tr>
            <td>&nbsp;</td>
            <td>Nota de Crédito</td>
            <td>Número de Transferência</td>
            <td>Valor (R$)</td>
            <td>Data Cadastro</td>
        </tr>
        </thead>
        <tbody></tbody>
    </table>

    <button class="btn btn-success btn-sm btn-float-right" id="saveNotacredito">
        <span class="glyphicon glyphicon-save" aria-hidden="true"></span>
        &nbsp;Salvar&nbsp;
    </button>
</div>