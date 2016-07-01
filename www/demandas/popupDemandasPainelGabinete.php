<?php

header("Content-Type: text/html; charset=ISO-8859-1",true);

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//total demandas atrasadas
$sql = "SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') as dmddatafimprevatendimento,
            ed.esddsc, d.docid
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND d.dmddatafimprevatendimento < CURRENT_DATE
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
            and d.celid = 49
        ";
$atrasados = $db->carregar( $sql );

//total demandas que vencem hoje
$sql = "SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') as dmddatafimprevatendimento,
            ed.esddsc, d.docid
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
            and d.celid = 49
        ";
$emDia = $db->carregar( $sql );

//total demandas em dia
$sql = "SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') as dmddatafimprevatendimento,
            ed.esddsc, d.docid
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') > to_char(CURRENT_DATE::date,'YYYY-MM-DD')
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
            and d.celid = 49
        ";
$aVencer = $db->carregar( $sql );

//total demandas pausadas
$sql = "SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') as dmddatafimprevatendimento,
            ed.esddsc, d.docid
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
            and d.celid = 49
        ";
$pausadas = $db->carregar( $sql );
?>

<style type="text/css">
	.icon-green {
	color: green;
	}
	.icon-red {
	color: red;
	}
</style>

<table class="table table-hover table-bordered">
    <thead>
        <tr>
            <th style="text-align: center">Ações</th>
            <th style="text-align: center">Demanda</th>
            <th style="text-align: center">Início</th>
            <th style="text-align: center">Fim</th>
        </tr>
    </thead>
    <tbody>
        <?php if($atrasados) foreach ( $atrasados as $dados) { ?>
            <tr class="danger">
                <td style="text-align: center">
                    <a href="?finalizar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Finalizar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ok icon-green"></span></a>&nbsp;&nbsp;<a href="?cancelar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Cancelar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ban-circle icon-red"></span></a>
                </td>
                <td style="text-align: left"><?php echo $dados['dmdid'] . ' - ' . $dados['dmdtitulo']; ?></td>
                <td><?php echo $dados['dmddatainiprevatendimento']; ?></td>
                <td><?php echo $dados['dmddatafimprevatendimento']; ?></td>
            </tr>
        <?php } ?>
        <?php if($emDia) foreach ($emDia as $dados) { ?>
            <tr class="warning">
                <td style="text-align: center">
                    <a href="?finalizar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Finalizar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ok icon-green"></span></a>&nbsp;&nbsp;<a href="?cancelar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Cancelar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ban-circle icon-red"></span></a>
                </td>
                <td style="text-align: left"><?php echo $dados['dmdid'] . ' - ' . $dados['dmdtitulo']; ?></td>
                <td><?php echo $dados['dmddatainiprevatendimento']; ?></td>
                <td><?php echo $dados['dmddatafimprevatendimento']; ?></td>
            </tr>
        <?php } ?>
        <?php if($aVencer) foreach ($aVencer as $dados) { ?>
            <tr class="success">
                <td style="text-align: center">
                    <a href="?finalizar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Finalizar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ok icon-green"></span></a>&nbsp;&nbsp;<a href="?cancelar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Cancelar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ban-circle icon-red"></span></a>
                </td>
                <td style="text-align: left"><?php echo $dados['dmdid'] . ' - ' . $dados['dmdtitulo']; ?></td>
                <td><?php echo $dados['dmddatainiprevatendimento']; ?></td>
                <td><?php echo $dados['dmddatafimprevatendimento']; ?></td>
            </tr>
        <?php } ?>
        <?php if($pausadas) foreach ($pausadas as $dados) { ?>
            <tr class="pausada">
                <td style="text-align: center">
                    <a href="?finalizar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Finalizar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ok icon-green"></span></a>&nbsp;&nbsp;<a href="?cancelar=1&dmdid=<?php echo $dados['dmdid']; ?>" title="Cancelar a Demanda" class="link_acoes"><span class="glyphicon glyphicon-ban-circle icon-red"></span></a>
                </td>
                <td style="text-align: left"><?php echo $dados['dmdid'] . ' - ' . $dados['dmdtitulo']; ?></td>
                <td><?php echo $dados['dmddatainiprevatendimento']; ?></td>
                <td><?php echo $dados['dmddatafimprevatendimento']; ?></td>
            </tr>
        <?php } ?>
    </tbody>
</table>

<?

function montaPausaDemanda($dmdid){
	global $db;

	
				//verifica pausa da demanda
				$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, 
							   replace(substr(p.pdmjustificativa, 0 , 4000), chr(13)||chr(10), '<br>') as pdmjustificativa,
							   to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
						from demandas.pausademanda p 
						inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
						where p.pdmstatus = 'A' and p.dmdid = ". (int) $dmdid."
						order by p.pdmid desc limit 1";
						
				$dadosp = $db->carregar($sql);	
				
				$flagIndeterminado = '';
				$tempototalpausa = 0;
				$textotempopausa = "<div align='left' style='background-color: #CFCFCF;font-size: 12px;'>";
				
				if($dadosp){
					foreach($dadosp as $dadop){
						
						if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){
							
							$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
							$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
							$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
							$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
							$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
				
							$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
							$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
							$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
							$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
							$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);
							
							$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
							$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final
							
							// pega o tempo total da pausa
							$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);
							
							
							$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
							$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';
							
						}
	
						//monta o texto da tempopausa
						//$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
						$textotempopausa .= "<b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
						$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
						if($dadop['datapausafim']){
							$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
						}else{
							$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
						}
						
						//$textotempopausa .= "<br><br>";
					}
					
				}	
					

				$textotempopausa .= "</div>"; 
	 
				echo $textotempopausa;
	
}
?>

<script type="text/javascript">
    $(function(){
        $('.link_acoes').click(function(){
            if(!confirm('Deseja realmente ' + $(this).attr('title'))){
                return false;
            }
        });
    });        
</script>