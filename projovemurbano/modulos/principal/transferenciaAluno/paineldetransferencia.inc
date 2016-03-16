<?php

monta_titulo('','Dados da Transferência');
$_SESSION['projovemurbano']['modalidade'] = '';

$bloq = 'S'

?>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
    
    jQuery('.pesquisar').click(function(){
        jQuery('#polid  option').attr('selected',true);
        jQuery('#nucid  option').attr('selected',true);
        jQuery('#turid  option').attr('selected',true);
        jQuery('#shtid  option').attr('selected',true);
        jQuery('#caecpf option').attr('selected',true);
        jQuery('#requisicao').val('');
        jQuery('#form').submit();
    });
    
    jQuery('.detalhe').live('click',function(){
        var traid = jQuery(this).attr('id');

        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "&requisicao=mostraDetalheTransferencia&traid="+traid,
            async: false,
            success: function(msg){

                jQuery( "#dialog-detalhe" ).html(msg);       
                jQuery( "#dialog-detalhe" ).dialog({
                    resizable: false,
                    height:500,
                    width:800,
                    modal: true,
                    show: { effect: 'drop', direction: "up" },
                    buttons: {
                        "OK": function() {
                            jQuery( this ).dialog( "close" );                            
                        }
                    }
                });
            }
        });
    });
    
    jQuery('.baixar').click(function(){
        jQuery('#arqid').val(jQuery(this).attr('id'));
        jQuery('#form').submit();
    });
    
});
function filtraNucleo(){
    jQuery('#requisicao').val('buscarComboNucleo');
    jQuery('#polid option').attr('selected',true);
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "&"+jQuery('#form').serialize(),
        async: false,
        success: function(msg){
            jQuery('#tr_nucid').remove();
            jQuery('#tr_polid').after(msg);
        }
    });
}

function filtraTurma(){
    jQuery('#requisicao').val('buscarComboTurma');
    jQuery('#nucid option').attr('selected',true);
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "&"+jQuery('#form').serialize(),
        async: false,
        success: function(msg){
            jQuery('#tr_turid').remove();
            jQuery('#tr_nucid').after(msg);
        }
    });
}
    
    
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
    var div_on = document.getElementById( campo + '_campo_on' );
    var div_off = document.getElementById( campo + '_campo_off' );
    var input = document.getElementById( campo + '_campo_flag' );
    if ( div_on.style.display == 'none' )
    {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else
    {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}

</script>
<style>
.div_muda{
align:top;
}
</style>
<?
if($_SESSION['projovemurbano']['ppuid']!=1){
	$filtrotprid = "AND tprid = '".$_SESSION['projovemurbano']['tprid']."'";
}
$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' $filtrotprid AND pmustatus='A'";
$polomunicipio = $db->pegaLinha($sql);
?>  
<div id="dialog-detalhe"></div>
<form id="form" name="form" method="POST" >
    <input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="" name="arqid" id="arqid" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
               <tr>
            <td class="SubTituloCentro" width="50%" colspan="2">Filtros</td>
        </tr>
        <tr>
            <td colspan="2">
                <table width="100%"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
                    <?
                    if( $polomunicipio['pmupossuipolo'] == 't' ){
                        buscarComboPolo();
				    }
                    
                    buscarComboNucleo();
                    
                    buscarComboTurma();
                           
                    $sql = "SELECT
                                shtid as codigo,
                                shtdescricao as descricao
                            FROM
                                projovemurbano.statushistorictransferencia
                            ORDER BY
                                2";
                    
                    mostrarComboPopup( 'Status:', 'shtid',  $sql, '', 'Selecione o(s) Status' );
                    ?>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="25%">CPF:
                            <input type="hidden" id="pmupossuipolo" value="<?=$polomunicipio['pmupossuipolo'] ?>" />
                        </td>
                        <td width="75%">
                            <? echo campo_texto('caecpf', "N","S", "CPF", 16, 14, "", "", '', '', 0, 'id="caecpf" required', '', '', ');' ); ?>
                        </td>
                    </tr>
                </table>
            </td>
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" class="pesquisar" value="Pesquisar">
            </td>
        </tr>
    </table>
</form>
<?
monta_titulo('Alunos a transferir','');
if($_SESSION['projovemurbano']['ppuid']!=1){
	$inner_filtrotprid = " INNER  JOIN projovemurbano.nucleo 			nucleo ON nucleo.nucid = cae.nucid AND nucleo.tprid = '".$_SESSION['projovemurbano']['tprid']."'";

}
$whereD = Array("pju.pjuid = {$_SESSION['projovemurbano']['pjuid']}");
$whereO = Array("pjuO.pjuid = {$_SESSION['projovemurbano']['pjuid']}");

extract($_POST);
// ver($caecpf,d);
if($caecpf[0] != ''){
   $cpfA = "AND cae.caecpf = '{$caecpf}'"; 
   $cpfB = "AND cae.caecpf = '{$caecpf}'"; 
}

if( $polid[0] != '' ){
    $whereD = Array("pol.polid IN (".implode(', ',$polid).")");
    $whereO = Array("polO.polid IN (".implode(', ',$polid).")");
}

if( $nucid[0] != '' ){
    $whereD = Array("nuc.nucid IN (".implode(', ',$nucid).")");
    $whereO = Array("nucO.nucid IN (".implode(', ',$nucid).")");
}

if( $turid[0] != '' ){
    $whereD = Array("tra.turid_origem IN (".implode(', ',$turid).")");
    $whereO = Array("tra.turid_destino IN (".implode(', ',$turid).")");
}

if( $shtid[0] != '' ){
    $whereD = Array("sht.shtid IN (".implode(', ',$shtid).")");
    $whereO = Array("sht.shtid IN (".implode(', ',$shtid).")");
}
    
$sql = "SELECT 
            shtdescricao as status,            
            cae.caecpf as cpf,
            cae.caenome as nome,
            ( coalesce(estO.estuf,munO.estuf||' - '||munO.mundescricao) ) as localorigem,
            'Polo: '||tra.polid_origem as polo_origem,
            'Núcleo: '||tra.nucid_origem as nucleo_origem,
            (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_origem
                --AND    turstatus = 'A'
    		)  as turma_origem,
            (   SELECT
                    ppuano as ano2
                FROM
                    projovemurbano.programaprojovemurbano ppu
                WHERE   ppu.ppuid = tra.ppuid_origem) as AnoOrigem,
            ( coalesce(est.estuf,mun.estuf||' - '||mun.mundescricao) ) as localdestino,
            'Polo: '||tra.polid_destino as polo_destino,
            'Núcleo: '||tra.nucid_destino as nucleo_destino,
            (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_destino
                --AND    turstatus = 'A'
    		) as turma_destino,
            (   SELECT
                    ppuano as ano2
                FROM
                    projovemurbano.programaprojovemurbano ppu
                WHERE   ppu.ppuid = tra.ppuid_destino) as AnoDestino,
            CASE WHEN arqt.arqid IS NULL
                THEN 'Não Existente'
                ELSE '<center><a id='||arqt.arqid||' style=cursor:pointer;color:blue class=baixar>Baixar</a></center>'
            END as arquivo,
            '<div class=detalhe id='|| tra.traid ||' >
                <img style=cursor:pointer width=55px src=/imagens/gadget_busca.png border=0 />
            </div>' as detalhe
        FROM 
            projovemurbano.transferencia tra 
        INNER JOIN seguranca.usuario                            usu  ON usu.usucpf  = tra.usucpfresponsavelorigem
        LEFT  JOIN seguranca.usuario                            usu2 ON usu2.usucpf = tra.usucpfresponsaveldestino
        INNER JOIN projovemurbano.cadastroestudante             cae  ON cae.caeid   = tra.cad_caeid 
        LEFT  JOIN projovemurbano.arquivo_tranferencia          arqt ON arqt.traid  = tra.traid
        INNER JOIN projovemurbano.historico_transferencia       hst  ON hst.htrid   = tra.htrid_ultimo
        INNER JOIN projovemurbano.statushistorictransferencia   sht  ON sht.shtid   = hst.shtid_status
        
    	$inner_filtrotprid
    	
        LEFT  JOIN projovemurbano.polo                          polO
            INNER JOIN projovemurbano.polomunicipio             pmuO  ON pmuO.pmuid = polO.pmuid                          
        ON polO.polid  = tra.polid_origem
        LEFT  JOIN projovemurbano.nucleo                        nucO  
            INNER JOIN projovemurbano.municipio                 muniO  ON muniO.munid = nucO.munid
            INNER JOIN projovemurbano.polomunicipio             pmuO2 ON pmuO2.pmuid = muniO.pmuid  
        ON nucO.nucid  = tra.nucid_origem 
        LEFT  JOIN projovemurbano.projovemurbano                pjuO ON pjuO.pjuid = pmuO.pjuid OR pjuO.pjuid = pmuO2.pjuid
        LEFT  JOIN territorios.municipio                        munO ON munO.muncod = pjuO.muncod
        LEFT  JOIN territorios.estado                           estO ON estO.estuf = pjuO.estuf
    
        LEFT  JOIN projovemurbano.polo pol
            INNER JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid                          
        ON pol.polid  = tra.polid_destino
        LEFT  JOIN projovemurbano.nucleo                        nuc  
            INNER JOIN projovemurbano.municipio                 muni  ON muni.munid = nuc.munid
            INNER JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = muni.pmuid  
        ON nuc.nucid  = tra.nucid_destino 
        LEFT  JOIN projovemurbano.projovemurbano                pju ON pju.pjuid = pmu.pjuid OR pju.pjuid = pmu2.pjuid
        LEFT  JOIN territorios.municipio                        mun ON mun.muncod = pju.muncod
        LEFT  JOIN territorios.estado                           est ON est.estuf = pju.estuf

        WHERE   
            (
                shtid_status IN (2,3)  $cpfA AND ".implode(" AND ",$whereD)."
            )
            OR
            (
                ".implode(" AND ",$whereO)." $cpfB $filtrotprid2
            )
        ORDER BY
            nome";
$cabecalho = array("Status","CPF Aluno","Aluno","Local Origem","Pólo Origem","Núcleo Origem","Turma Origem","Ano Origem","Local Destino",
                    "Pólo Destino","Núcleo Destino","Turma Destino","Ano destino","Arquivo Anexo","Detalhe");
                    
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', 'form_transferencias');
?>
 
<? registarUltimoAcesso(); ?>
