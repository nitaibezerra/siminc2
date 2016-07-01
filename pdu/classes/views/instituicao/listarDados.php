<?php
/**
 * Created by PhpStorm.
 * User: RuySilva
 * Date: 05/06/14
 * Time: 20:59
 */
?>
<table class="table table-striped table-hover table-bordered table-condensed" style="margin-bottom: 0px;">
<!--    <thead>-->
<!--    <tr>-->
<!--        <th class="text-center" colspan="1">Ação</th>-->
<!--        <th style="cursor: pointer;" class="load-listing-ajax-order" field="2">-->
<!--            Itens-->
<!--        </th>-->
<!--    </tr>-->
<!--    </thead>-->
    <tbody>
        <tr style="cursor: pointer;" onclick="exibirDadosInstituicao( <?php echo $this->id ?> , this )">
            <td class="text-center" style="width: 5%">
<!--                <a href="javascript:void(0);" >-->
<!--                    <i class="glyphicon glyphicon-plus"></i>-->
<!--                </a>-->
                <a href="javascript:void(0);">
                    <i class="glyphicon glyphicon-search"></i>
                </a>
            </td>
            <td>  Dados da instituição</td>
        </tr>
        <tr style="cursor: pointer;" onclick="javascript:exibirIndicadoresEducacionais( <?php echo $this->id ?> , this )">
            <td class="text-center">
                <a href="javascript:void(0);">
<!--                    <i class="glyphicon glyphicon-plus"></i>-->
                <i class="glyphicon glyphicon-hand-left"></i>
                </a>
            </td>
            <td>Indicadores educacionais</td>
        </tr>
        <tr style="cursor: pointer;"  onclick="javascript:exibirDiagnostico( <?php echo $this->id ?> , this)">
            <td class="text-center">
<!--                <i class="glyphicon glyphicon-plus"></i>-->
                <a href="javascript:void(0);">
                    <i class="glyphicon glyphicon-stats"></i>
                </a>
            </td>
            <td> Diagnóstico</td>
        </tr>
        <tr  style="cursor: pointer;"  onclick="listarCampus(<?php echo $this->id ?> , this)">
            <td class="text-center">
                <a href="javascript:void(0);">
                    <i class="glyphicon glyphicon-plus"></i>
                </a>
            </td>
            <td>Campus</td>
        </tr>
<!--        <tr>-->
<!--            <td>-->
<!---->
<!--            </td>-->
<!--        </tr>-->
    </tbody>
</table>
<form name="instituicao" method="post" action="/pdu/pdu.php?modulo=principal/instituicao&acao=C">
    <input type="hidden" name="id" value="<?php echo $this->id ?>">
</form>
<form name="indicadoresEducacionais" method="post" action="/pdu/pdu.php?modulo=principal/indicadores_educacionais&acao=C">
    <input type="hidden" name="id" value="<?php echo $this->id ?>">
</form>
<script>
    /**
     * rediceriona para tela
     */
    function exibirDadosInstituicao(id , element)
    {
        $(element).closest('table').next('form[name=instituicao]').submit();
    }

    /**
     * abrir modal
     */
    function exibirIndicadoresEducacionais(id , element)
    {
        $(element).closest('table').next().next('form[name=indicadoresEducacionais]').submit();
    }

    /**
     * exibir listagem na propria tela
     */
    function listarCampus(id , element)
    {
        $.post(window.location.href , { controller: 'campus' , action: 'listarCampiReitoria' , id: id} , function(html){
            $(element).closest('tr').after(function(){return $('<tr><td style="background-color: #FFF; width: 5%"></td><td style="background-color: #FFF" colspan="4">' + html + '</td></tr>').hide().toggle(300);});
//            $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="fecharCampus(\'' + id + '\' , this)"><i class="glyphicon glyphicon-minus"></i></a>');
            $(element).attr( 'onclick', 'fecharCampus(\'' + id + '\' , this)');
            $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);"><i class="glyphicon glyphicon-minus"></i></a>');
        });
    }
    /**
     * exibir listagem na propria tela
     */
    function fecharCampus(id , element)
    {
        console.info($(element).next());
        $(element).next().hide( function(){$(this).remove()} );
        $(element).attr( 'onclick', 'listarCampus(\'' + id + '\' , this)');
        $(element).find('td:first').html('<a href="javascript:void(0);"><i class="glyphicon glyphicon-plus"></i></a>');
//        $($(element).closest('tr').next()[0]).hide( function(){$(this).remove()} );
//        $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="listarCampus(\'' + id + '\' , this)"><i class="glyphicon glyphicon-plus"></i></a>');
    }

    /**
     * Ir para outra tela
     */
    function exibirDiagnostico()
    {
        $.post(window.location.href , {controller: 'diagnostico' , action: 'exibir'} , function(html){
            $('#modal').html(html).modal('show');
        });
    }
</script>