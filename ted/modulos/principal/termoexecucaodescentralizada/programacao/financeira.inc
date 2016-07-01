<?php
$financeiro = new Ted_Model_Financeiro();
$dadosFinanceiro = $financeiro->getByTcpid(Ted_Utils_Model::capturaTcpid());
$jsonDadosFinanceiros = json_encode(array('financeiro' => $dadosFinanceiro));
//ver($jsonDadosFinanceiros);
?>
<script id="table-financeiro" type="text/x-handlebars-template">
    <tr>
        <td>
            <span class="glyphicon glyphicon-remove cursor-click remove-nc newLine" data-action="financeiro" data-remove-id="{{previd}}"></span>
            <input type="hidden" value="{{previd}}" name="previd[]" id="previd-{{previd}}" />
        </td>
        <td class="text-center" width="">
            {{previd}}
        </td>
        <td class="text-center" width="">
            <span class="glyphicon glyphicon-edit" data-target-id="{{previd}}" aria-hidden="true"></span>
            {{prevdsc}}
        </td>
        <td class="text-center" width="">
            <input type="text" class="form-control" name="prevdata[]" id="prevdata-{{previd}}" value="{{prevdata}}">
        </td>
        <td>
            <input type="text" name="prevalor[]" maxlength="17" value="{{prevalor}}" id="prevalor_{{prevalor}}" class="form-control text-right" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
        </td>
        <td class="text-center" width="">
            <input type="text" class="form-control" name="prevpercentual[]" id="previd-{{previd}}" value="{{prevpercentual}}">
        </td>
    </tr>
</script>
<script type="text/javascript">
    $(function() {
        $("#addFinanceiro").on("click", function() {
            var templateString = Handlebars.compile($("#table-financeiro").html());
            actionFinanceiro(templateString);
        });

        loadGridFinanceiro();
        $("[name='prevalor[]']").focus().blur();
    });

    /**
     * Carga de dados para grid de Financeiro [Programação orçamentária]
     * segunda parte
     * @return void(0)
     */
    var loadGridFinanceiro = function() {
        var jsonStringfy = <?=$jsonDadosFinanceiros;?>;
        console.log(jsonStringfy);
        if (typeof jsonStringfy === 'object') {
            if (jsonStringfy.financeiro) {
                var template = Handlebars.compile($("#table-financeiro").html());
                jsonStringfy.financeiro.forEach(function(objData, i){
                    //console.log(objData);
                    actionFinanceiro(template, objData);
                });
            }
        }
    };

    var actionFinanceiro = function(template, objData) {
        var html
          , callMethods = ["getMesesExecucao"]
          , calls = []
          , objData = objData || {};

        objData["proid"] = objData.proid || new Date().getTime();
        html = template(objData);
        if ($("#tb_financeiro tbody tr").length) {
            $("#tb_financeiro tbody tr").last().after(html);
        } else {
            $("#tb_financeiro tbody").append(html);
        }

        for (var i=0; i < callMethods.length; i++) {
            calls.push($.ajax({
                url:settings.endpoint+"&action="+callMethods[i]+"&ted="+settings.ted
            }));
        }

        $.when.apply($, calls).done(function() {
            $(arguments).each(function(i, v) {
                if (i == 0)
                    $("#tb_financeiro tbody [name='crdmesexecucao[]']").last().append(v);

            });
        }).fail(function() {
            console.log('fail, insert new line');
        });
    };
</script>

<div class="row col-md-12">
    <button class="btn btn-primary btn-sm" id="addFinanceiro">
        <span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
        &nbsp;Adicionar
    </button>

    <table class="table table-bordered tb_render" id="tb_financeiro">
        <thead>
        <tr>
            <td>&nbsp;</td>
            <td>Etapa</td>
            <td>Descricao</td>
            <td>Prazo (quantidade de dias)</td>
            <td>Valor (R$)</td>
            <td>Percentual (%)</td>
        </tr>
        </thead>
        <tbody></tbody>
    </table>

    <button class="btn btn-success btn-sm btn-float-right" id="saveFinanceiro">
        <span class="glyphicon glyphicon-save" aria-hidden="true"></span>
        &nbsp;Salvar&nbsp;
    </button>
</div>