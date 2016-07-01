<?php

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Tela de Controle de Vigência', '' );

$sql = "select ptrcod, ptrexercicio, enbnome, estuf, ptrid, maxdata, dias, anasituacaoparecer, pagamento from(   
		    SELECT ptr.ptrcod, ptr.ptrexercicio, enb.enbnome, enb.estuf, ptv.ptrid, to_char(max(ptv.vigdatafim), 'DD/MM/YYYY') as maxdata, 
		        cast(to_char(case when cast(to_char(now(), 'YYYY-MM-DD') as date) >  max(ptv.vigdatafim) THEN
		             cast(to_char(now(), 'YYYY-MM-DD') as date) - max(ptv.vigdatafim) else 
		        '00' end , 'DD') as integer) as dias, 
		        case when ana.anasituacaoparecer = 'F' then 'Aprovado'
		        	when ana.anasituacaoparecer = 'D' then 'Indeferido'
		            when ana.anasituacaoparecer = 'E' then 'Em diligência'
		            when ana.anasituacaoparecer = 'C' then 'Empenho Condicionado'
		            else 'Aguardando'
		            end as anasituacaoparecer,
		        case when (doc.esdid = 120 or doc.esdid = 207) then 'Pago' else 'Não Pago' end as pagamento
		    FROM 
		        emenda.ptvigencia ptv
		        inner join emenda.planotrabalho ptr on ptr.ptrid = ptv.ptrid
		        inner join workflow.documento doc on doc.docid = ptr.docid
		        inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
		        left join emenda.analise ana on ana.ptrid = ptr.ptrid
		    WHERE
		        ptv.vigdatafim is not null
		        and ptr.ptrid in (select max(ptrid) from emenda.planotrabalho /*where ptrcod = 822*/ group by ptrcod)
		        and doc.esdid not in (245, 344, 120)
		        and ana.anastatus = 'A'
		        and ptv.vigstatus = 'A'
		        and ana.anatipo = 'T'
		        and ana.analote = (select max(analote) from emenda.analise
		        					where ptrid = ptr.ptrid
		                            	and anatipo = 'T')
		    GROUP BY
		        ptv.ptrid,
		        ptr.ptrcod,
		        ptr.ptrexercicio,
		        enb.enbnome,
		        enb.estuf,
		        ana.anasituacaoparecer,
		        doc.esdid
		    order by
		        ptr.ptrcod,
		        ptv.ptrid
		) as foo
		where
			dias not in (0)
		order by
			ptrcod";
//and ( (select sum(exfvalor) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumempenhooriginal is not null) > orb.orbvalorpagamento or orb.orbnumordembancaria is not null)
$arDados = $db->carregar( $sql );

$cabecalho = array("Nº PTA", "Data Vigência", "Dias de Atraso", "Entidade", "Estado", "Situação da Análise", "Pagamento");
?>
<form name="formconvenio" id="formconvenio" action="" enctype="multipart/form-data" method="post">

<table class="listagem" align="center" bgcolor="#f5f5f5" border="0" cellpadding="2" cellspacing="0" width="95%">
	<thead>
	<tr>
		<?
		foreach ($cabecalho as $cab) {
			echo '<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" 
					onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" align="Center" bgcolor="" 
					valign="top"><strong>'.$cab.'</strong></td>';
		}		
		?>
	</tr>
	</thead>
	<?
	if($arDados){
		foreach ($arDados as $key => $v) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			echo '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
					  <td align="left">'.$v['ptrcod'].'/'.$v['ptrexercicio'].'</td>
					  <td align="left">'.$v['maxdata'].'<br></td>
					  <td style="color: red;" align="right">'.$v['dias'].'</td>
					  <td align="left">'.$v['enbnome'].'<br></td>
					  <td align="left">'.$v['estuf'].'<br></td>			  
					  <td align="left">'.$v['anasituacaoparecer'].'</td>			  
					  <td align="left">'.$v['pagamento'].'</td>			  
				  </tr>';
		}
		totalizadorRegistroLista(sizeof($arDados));
	} else{
		echo '<tr>
				  <td colspan="7"><table width="100%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
					</table>
				  </td>			  
			  </tr>';
	}
	?>
	</table>