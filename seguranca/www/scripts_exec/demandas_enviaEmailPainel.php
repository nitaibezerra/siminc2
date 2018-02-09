<?php
 
// configurações
ini_set("memory_limit", "3000M");
set_time_limit(30000);

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "config.inc";
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ . "www/demandas/_constantes.php";
include_once APPRAIZ . "www/demandas/_funcoes.php";


//recupera analistas da celula b - daniel brito 
$sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome,
                    u.usuemail
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    and ur.pflcod in (237)
                    and ur.celid = 2
                ORDER BY u.usunome";
$rs = $db->carregar($sql);

if($rs){
	
	foreach($rs as $dados){
		
		
		//total demandas atrasadas
		$sql = "SELECT
		            d.dmdid,
		            d.dmdtitulo,
		            u.usunome,
		            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
		            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
		            ed.esddsc
		        FROM
		            demandas.demanda as d
		        LEFT JOIN
		            workflow.documento doc ON doc.docid       = d.docid
		        LEFT JOIN
		            workflow.estadodocumento ed ON ed.esdid = doc.esdid
		        LEFT JOIN
		            seguranca.usuario u ON u.usucpf = d.usucpfexecutor
		        WHERE
		            d.usucpfanalise = '".$dados['usucpf']."'
		            AND d.usucpfdemandante is not null
		            AND d.dmdstatus = 'A'
		            AND ed.esdstatus = 'A'
		            AND doc.esdid in (91,92,107,108)
		            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') <= to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
		            AND to_char(d.dmddatainiprevatendimento::date,'YYYY') > '2012'
		        ORDER BY
		        	d.dmddatafimprevatendimento";
		$atrasados = $db->carregar( $sql );
		
		if($atrasados){
			
			
			$remetente = array('nome'=>'DEMANDAS EM ATRASO', 'email'=>$_SESSION['email_sistema']); 

			$destinatario = $dados['usuemail'];
			
			$assunto = "Você possui demandas em atraso - " . date("d/m/Y");

			$html = htmlMail('P',$atrasados);
			
			$conteudo = $dados['usunome'].',	
						<br><br>Você possui '.count($atrasados).' demanda(s) em atraso(s) ou que vence(m) hoje. Favor atualize suas demandas.
						<br><br>' . $html;
			//dbg($conteudo,1); 
			 
			enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

			
		}
		
		
		
	}
}


//recupera programadores da celula b - daniel brito 
$sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome,
                    u.usuemail
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    and ur.pflcod in (238)
                    and ur.celid = 2
                ORDER BY u.usunome";
$rs = $db->carregar($sql);

if($rs){
	
	foreach($rs as $dados){
		
		
		//total demandas atrasadas
		$sql = "SELECT
		            d.dmdid,
		            d.dmdtitulo,
		            u.usunome,
		            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
		            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
		            ed.esddsc
		        FROM
		            demandas.demanda as d
		        LEFT JOIN
		            workflow.documento doc ON doc.docid       = d.docid
		        LEFT JOIN
		            workflow.estadodocumento ed ON ed.esdid = doc.esdid
		        LEFT JOIN
		            seguranca.usuario u ON u.usucpf = d.usucpfanalise
		        WHERE
		            d.usucpfexecutor = '".$dados['usucpf']."'
		            AND d.usucpfdemandante is not null
		            AND d.dmdstatus = 'A'
		            AND ed.esdstatus = 'A'
		            AND doc.esdid in (91,92,107,108)
		            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') <= to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
		            AND to_char(d.dmddatainiprevatendimento::date,'YYYY') > '2012'
		        ORDER BY
		        	d.dmddatafimprevatendimento";
		$atrasados = $db->carregar( $sql );
		
		if($atrasados){
			
			
			$remetente = array('nome'=>'DEMANDAS EM ATRASO', 'email'=>$_SESSION['email_sistema']); 

			$destinatario = $dados['usuemail'];
			
			$assunto = "Você possui demandas em atraso - " . date("d/m/Y");

			$html = htmlMail('A',$atrasados);
			
			$conteudo = $dados['usunome'].',	
						<br><br>Você possui '.count($atrasados).' demanda(s) em atraso(s) ou que vence(m) hoje. Verifique as demandas com seu analista.
						<br><br>' . $html;
			//dbg($conteudo,1); 
			//$emailCopia	= array($_SESSION['email_sistema']);
			 
			enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

			
		}
		
		
		
	}
}


function htmlMail( $tipo = NULL, $arrayDemandas = NULL ) {
    $text = '<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:w="urn:schemas-microsoft-com:office:word" xmlns:m="http://schemas.microsoft.com/office/2004/12/omml" xmlns="http://www.w3.org/TR/REC-html40">
			<head>
			<style>
			.table_mail{
			    width: 80%;
			    border:outset #0099CC 2px;
			}
			
			.tit_td{
				background:#6699CC; 
				font-size:10.0pt;
				font-family:"Arial","sans-serif";
				color:white;
				font-weight: bold;
				text-align: center;
				border-bottom: 1px solid white;
				margin:2px;
			}
			
			.item_1{			
				font-size:9.0pt;
				text-align: right;
				font-weight: bold;
				font-family:"Arial","sans-serif";
				padding: 3px;
				padding-right: 5px;
				border-right: 1px solid white;
			}
			
			.item_2{
				font-size:9.0pt;			
				text-align: left;
				font-family:"Arial","sans-serif";
				padding-left: 3px;
				border-right: 1px solid white;	
			}
			
			</style>
			</head>
			<body lang=PT-BR link=blue vlink=purple>
			    <table cellpadding="1" cellspacing="0" class="table_mail">
			    	<tr>
			    		<td class="tit_td" colspan="4">DEMANDAS</td>
			    	</tr>';

    if ( $arrayDemandas ) {
    	
    	$titulo = ($tipo == 'A' ? 'Analista' : 'Programador');
    	
        $text .= '
			    	<tr>
			    		<td colspan="4">
			    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
						    	<tr>
						    		<td class="tit_td">Código</td>
						    		<td class="tit_td">Demanda</td>
						    		<td class="tit_td">'.$titulo.'</td>
						    		<td class="tit_td">Início</td>
						    		<td class="tit_td">Fim</td>
						    		<td class="tit_td">Status</td>
						    	</tr>';
        $a = 0;
        foreach ( $arrayDemandas as $val ) {
            $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
			    		<td class="item_2">' . $val['dmdid'] . '</td>
			    		<td class="item_2">' . $val['dmdtitulo'] . '</td>
			    		<td class="item_2">' . $val['usunome'] . '</td>
			    		<td class="item_2">' . $val['dmddatainiprevatendimento'] . '</td>
			    		<td class="item_2">' . $val['dmddatafimprevatendimento'] . '</td>
			    		<td class="item_2">' . $val['esddsc'] . '</td>
			    	  </tr>';
            $a++;
        }

        $text .= '
					    </table>    	   
		    		</td>
		    	</tr>';
    }


    $text .= '</table>
			  </body>
			  </html>';

    return $text;
}
	
?>