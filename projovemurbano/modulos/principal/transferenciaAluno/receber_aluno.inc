<?php
    monta_titulo('','Dados da Transferência');
    
    $bloq = "S";
    $bloq = 'S'
?>
<?
// $sql = "SELECT 
// 			perdtfim
// 		FROM 
// 			projovemurbano.periodocurso
// 		WHERE
// 			perid = (SELECT DISTINCT
// 		      MAX (per.perid)
// 		 FROM
// 		    projovemurbano.diario dia
// 		 INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
// 		 WHERE
// 			to_char(perdtfim,'YYYYMMDD') <= to_char(now(),'YYYYMMDD'))";

// $dataUltimoPeriodo = $db->pegaUm($sql);
// $aposSeisDias      = formata_data( somar_dias_uteis( $dataUltimoPeriodo, 6 ) );
// $aposOitoDias	   = formata_data( somar_dias_uteis( $dataUltimoPeriodo, 8 ) );

//     $sql="SELECT 
//             true
//          FROM 
//             projovemurbano.periodocurso
//          WHERE 
//             to_char($aposSeisDias,'YYYYMMDD') <= to_char(now(),'YYYYMMDD')            
//          AND to_char($aposOitoDias,'YYYYMMDD') >= to_char(now(),'YYYYMMDD')";

//    $permiteTransferencia = $db->pegaUm($sql); 
   
   $sql2 = "SELECT
                true
            FROM projovemurbano.transferencia   
            WHERE 
                traid is not null";
    $checaTraid = $db->pegaUm($sql);
    
?>
<style type="">
.ui-dialog-titlebar{
text-align: center;
}
</style>
<script language="javascript" type="text/javascript">
    jQuery(document).ready(function(){  
          
        jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="requisicao" id="requisicao" value=""/>');
          
        jQuery('.chk').live('click',function(){
            var traid = jQuery(this).val();
            if( jQuery(this).attr('checked') ){
                var nucid = jQuery(this).attr('id');
                jQuery.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "requisicao=buscarTurmas&nucid=" + nucid + "&bloq=<?=$bloq ?>",
                    async: false,
                    success: function(msg){
                        jQuery('#div_turma_'+traid).html(msg);
                        jQuery('#div_turma_'+traid).children(':first').attr('id','turid_'+traid);
                        jQuery('#div_turma_'+traid).children(':first').attr('name','turid['+traid+']');
                    }
                });
            }else{
                jQuery('#div_turma_'+traid).html(' ');
            }
        });
        
        jQuery('.tranferir').click(function(){
            // Valida campos obrigatórios
            if( jQuery('.chk:checked').length < 1 ){
                alert('Selecione pelo menos 1 aluno');
                return false;
            }
            var erro = false;
            jQuery('.obrigatorio').each(function(){
                if( jQuery(this).val() == '' ){
                    alert('Campo obrigatório.');
                    jQuery(this).focus();
                    erro = true;
                    return false;
                }
            });
            if( erro ){
                return false;
            }
            //FIM Valida campos obrigatórios
            jQuery('#requisicao').val('receber_aluno_tranferir');
            jQuery('#formTranferencia').submit();
        });
        
        jQuery('.cancelar').click(function(){
            if( jQuery('#tr_justificativa').css('display') == 'none' ){
                jQuery('#tr_justificativa').show();
                alert('Preencha a justificativa.');
                return false;
            }else{
                // Valida campos obrigatórios
                if( jQuery('.chk:checked').length < 1 ){
                    alert('Selecione pelo menos 1 aluno');
                    return false;
                }
                if( jQuery('#justificativa').val() == '' ){
                    alert('Preencha a justificativa.');
                    jQuery('#justificativa').focus();
                    return false;
                }
                jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="justificativa" value="'+jQuery('#justificativa').val()+'"/>');
                //FIM Valida campos obrigatórios
                jQuery('#requisicao').val('receber_aluno_cancelar');
                jQuery('#formTranferencia').submit();
            }
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
            jQuery('#formTranferencia').children(':first').before('<input type="hidden" name="arqid" value="'+jQuery(this).attr('id')+'"/>');
            jQuery('#formTranferencia').submit();
        });
    });
</script>	
<?
if($_SESSION['projovemurbano']['ppuid']!=1){
	$filtrotprid = "AND tprid = '".$_SESSION['projovemurbano']['tprid']."'";
}
$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' $filtrotprid AND pmustatus='A'";
$polomunicipio = $db->pegaLinha($sql); 

monta_titulo('Alunos a transferir','');

$sqlano = " SELECT
                ppuano as ano2
             FROM
                projovemurbano.programaprojovemurbano ppu
            WHERE ";


$sql = "SELECT 
            '<center><input type=checkbox name=traid[] value='|| tra.traid ||' id='|| tra.nucid_destino ||' class=chk />
                     <div id=div_turma_'|| tra.traid ||' ></div>
            </center>' as marcar, 
            shtdescricao as status,            
            cae.caecpf as cpf,
            cae.caenome as nome,
            (SELECT DISTINCT
                coalesce(e.estdescricao,m.mundescricao)
            FROM 
                   territorios.municipio        m 
            LEFT  JOIN territorios.estado e ON e.estuf = m.estuf
            LEFT  JOIN projovemurbano.municipio mun ON mun.muncod = m.muncod
            LEFT  JOIN projovemurbano.nucleo nuc ON nuc.munid = mun.munid
            WHERE
                nuc.nucid = tra.nucid_origem) as LocalOrigem,
            ( $sqlano  ppu.ppuid = tra.ppuid_origem) as AnoOrigem,
            CASE 
                WHEN (SELECT muncod FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}) != ''
                    THEN 'Municipal'
                WHEN (SELECT estuf FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}) != ''
                    THEN 'Estadual'
                ELSE 'Sem Registro' 
            END as esfera,
            ( $sqlano  ppu.ppuid = tra.ppuid_destino) as AnoDestino,
            CASE WHEN arqt.arqid IS NULL
                THEN 'Não Existente'
                ELSE '<center><a id='||arqt.arqid||' style=cursor:pointer;color:blue class=baixar>Baixar</a></center>'
            END as arquivo,
            '<div class=detalhe id='|| tra.traid ||' >
                <img style=cursor:pointer width=55px src=/imagens/gadget_busca.png border=0 />
            </div>' as detalhe
        FROM 
            projovemurbano.transferencia tra 
        INNER JOIN projovemurbano.cadastroestudante             cae  ON cae.caeid  = tra.cad_caeid 
        LEFT  JOIN projovemurbano.arquivo_tranferencia          arqt ON arqt.traid = tra.traid
        INNER JOIN projovemurbano.historico_transferencia       hst  ON hst.htrid  = tra.htrid_ultimo
        INNER JOIN projovemurbano.statushistorictransferencia   sht  ON sht.shtid  = hst.shtid_status
        LEFT  JOIN projovemurbano.polo pol
            INNER JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid                          
        ON pol.polid  = tra.polid_destino
        LEFT  JOIN projovemurbano.nucleo                        nuc  
            INNER JOIN projovemurbano.municipio                 mun  ON mun.munid = nuc.munid
            INNER JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = mun.pmuid  
        ON nuc.nucid  = tra.nucid_destino 
        WHERE   
            hst.shtid_status = 2 
            AND ( 
                pmu.pjuid = {$_SESSION['projovemurbano']['pjuid']} 
                OR pmu2.pjuid = {$_SESSION['projovemurbano']['pjuid']} 
                )
        ORDER BY
            nome ";

// ver($sql,d); 
$cabecalho = array("&nbsp","Status","CPF Aluno","Aluno","Local Origem","Ano Origem","Esfera Origem","Ano Destino","Arquivo Anexo", "Detalhe");
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', 'formTranferencia');
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center">
    <tr id="tr_justificativa" style="display:none">
        <td class="SubTituloDireita" width="25%" >Texto Adicional:</td>
        <td>
            <?=campo_textarea("justificativa","N","S","Digite o outro motivo", 60, 3, 200,"","","","","")?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloCentro">
      	<?if(!in_array ( PFL_CONSULTA, $perfis )
//       			$permiteTransferencia =='t'
			){?> 
            <input type="button" class="tranferir" value="Transferir">
            <input type="button" class="cancelar" value="Cancelar Transfência">
        <?}?>
        </td>
    </tr>
</table>

<div id="dialog-detalhe" title="Solicitações do Processo" style="display:none;">
</div>
