<?php

function listarRespostaQuestionarioEja( $request ){
    global $db;

    extract($request);

    $sql = "
        SELECT  quetitulo
        FROM questionario.questionarioresposta qrp
        INNER JOIN questionario.questionario que ON que.queid = qrp.queid
        WHERE qrp.qrpid = {$qrpid}
    ";
    $tituloQuestionairo = $db->pegaUm($sql);

    $sql = "
        SELECT  coalesce(mun.estuf||' - '||mun.mundescricao, est.estdescricao) as unidade
        FROM par.instrumentounidade inu

        LEFT  JOIN territorios.estado est ON est.estuf = inu.estuf
        LEFT  JOIN territorios.municipio mun ON mun.muncod = inu.muncod
        INNER JOIN par.pfadesaoprograma adp ON  adp.inuid = inu.inuid
        INNER JOIN par.pfquestionario que ON que.adpid = adp.adpid
        WHERE que.qrpid = {$qrpid}
    ";
    $unidade = $db->pegaUm($sql);
?>
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
            <tr>
                <td class="SubTituloDireita">
                    <center><?= $tituloQuestionairo ?></center>
                </td>
            </tr>
            <tr>
                <td bgcolor="#e9e9e9">
                    <center><?= $unidade ?></center>
                </td>
            </tr>
        </table>
<?php 
	$sql = "
            SELECT  per.perid,
                    per.pertitulo as pergunta,
                    coalesce(res.resdsc,replace(replace(ip.itptitulo,'7.1 ',''),'7.2 ','') ) as resposta,
                    p.pertitulo as subper,
                    r2.resdsc as subres
            FROM questionario.questionarioresposta qrp
            
            INNER JOIN questionario.pergunta per ON per.queid = qrp.queid
            LEFT JOIN questionario.resposta res ON res.qrpid = qrp.qrpid AND res.perid = per.perid
            LEFT join questionario.itempergunta ip ON ip.itpid = res.itpid
            LEFT join questionario.grupopergunta gp ON gp.itpid = ip.itpid
            LEFT join questionario.pergunta p on p.grpid = gp.grpid
            LEFT join questionario.resposta r2 on r2.perid = p.perid
            
            WHERE qrp.qrpid = $qrpid
                
            ORDER BY per.perid
        ";
	$arrDados = $db->carregar($sql);
	
	$arrLista = Array();
	$temp = '';
	foreach($arrDados as $k => $dados){
		if( $temp != $dados['perid'] ){
			$temp = $dados['perid'];
			$arrLista[] = Array('pergunta' => $dados['pergunta'], 'resposta' => $dados['resposta']);
			if( $temp == 3007 && $dados['resposta'] == 'Sim' ){
				$arrLista[] = Array('pergunta' => $arrDados[$k]['subper'], 'resposta' => $arrDados[$k]['subres']);
				$arrLista[] = Array('pergunta' => $arrDados[$k+1]['subper'], 'resposta' => $arrDados[$k+1]['subres']);
				$arrLista[] = Array('pergunta' => $arrDados[$k+2]['subper'], 'resposta' => $arrDados[$k+2]['subres']);
			}
		}
	}
	$cabecalho = array("Pergunta", "Resposta");
	$db->monta_lista($arrLista,$cabecalho,50000,1,'N','center','', '', $tamanho, $alinhamento);
}

function listarAdesoesEja( $request ){
    global $db;

    extract($request);

    $where = Array("qrp.queid = 78","adp.pfaid = 10");
    if( $esfera == 'E' ){
        $join = 'INNER JOIN territorios.estado est ON est.estuf = inu.estuf';
        $campo = 'est.estdescricao';
        $titulo = 'UF';
        if( $uf[0] != '' ){
                $where[] = "inu.estuf in ('".implode("', '", $uf)."')";
        }
    }else{
        $join = 'INNER JOIN territorios.municipio mun ON mun.muncod = inu.muncod';
        $campo = 'mun.estuf||\' - \'||mun.mundescricao as municipio';
        $titulo = 'Município';
        if( $muncod[0] != '' ){
            $where[] = "inu.muncod in ('".implode("', '", $muncod)."')";
        }
        if( $uf[0] != '' ){
            $where[] = "mun.estuf in ('".implode("', '", $uf)."')";
        }
    }

    $sql = "
        SELECT  $campo,
                esd.esddsc,
                '<center><img align=\"absmiddle\" src=\"/imagens/lista_verde.gif\" 
                title=\"Vizualizar Questionário\" style=\"cursor:pointer\" class=\"questionario\" 
                id=\"'||qrp.qrpid||'\"></center>' as acao

        FROM par.instrumentounidade inu

        $join

        INNER JOIN par.pfadesaoprograma adp ON  adp.inuid = inu.inuid
        INNER JOIN workflow.documento doc ON doc.docid = adp.docid 
        INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        INNER JOIN par.pfquestionario que ON que.adpid = adp.adpid
        INNER JOIN questionario.questionarioresposta qrp ON que.qrpid = qrp.qrpid 

        WHERE ".implode(" AND ",$where);
    //ver($sql, d);
    $cabecalho = array($titulo, "Situação", "Questionário");
    $db->monta_lista($sql,$cabecalho,50000,1,'N','center','', '', $tamanho, $alinhamento);
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND regcod in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
					muncod AS codigo,
					regcod ||' - '|| mundsc AS descricao
				FROM
					public.municipio
				Where munstatus = 'A'
				$stAnd
				ORDER BY 2 ASC;";
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Avaliação - EJA', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
		<title>Relatório</title>
		<script src="../includes/calendario.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript">
		
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
		
		function filtraMunicipio(){
		
			mostraMunicipio();
		
			var listauf = "";
			$('#uf').children().each(function(){
				listauf = listauf + "," + $(this).val();
			});
		
			if(listauf) listauf = listauf.substr(1);
			
			if(!listauf) listauf = 'XX';
			
			$.ajax({
				type: "POST",
				url: window.location,
				data: '&ajaxUf='+listauf,
				async: false,
				success: function(msg){
					$('#td_municipio').html(msg);
				}
			});
		
		}
		
		function mostraMunicipio(){
		
			var tr_municipio   = document.getElementById('tr_municipio');
			var td_municipio   = document.getElementById('td_municipio');
		
			if( document.formulario.esfera[1].checked == true ){
				tr_municipio.style.display = '';
				td_municipio.style.display = '';
		
			}else{
				td_municipio.style.display = 'none';
				tr_municipio.style.display = 'none';
			}
			
		}
		
		$(document).ready(function(){
			$('.pesquisar').click(function(){
				selectAllOptions( formulario.uf );
				selectAllOptions( formulario.muncod );
				$.ajax({
					type: "POST",
					url: window.location,
					data: "&req=listarAdesoesEja&"+$('#formulario').serialize(),
					async: false,
					success: function(msg){
						$('#td_lista').html(msg);
					}
				});
			});
			$('.questionario').live('click',function(){
				var qrpid = $(this).attr('id');
				$.ajax({
					type: "POST",
					url: window.location,
					data: "&req=listarRespostaQuestionarioEja&qrpid="+qrpid,
					async: false,
					success: function(msg){
						$('#td_questionario').html(msg);
					}
				});
			});
			$('.limpar').click(function(){
				window.location = window.location;
			});
		});
		
		</script>
	</head>
	<body>
		<form name="formulario" id="formulario" action="" method="post">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
				<tr>
					<td id="td_questionario" colspan="2">
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top">Esfera:</td>	
					<td>
						<input type="radio" value="E" name="esfera" onclick="mostraMunicipio();" onchange="mostraMunicipio();" checked="checked">Estadual
						&nbsp;&nbsp;  
						<input type="radio" value="M" name="esfera" onclick="mostraMunicipio();" onchange="mostraMunicipio();">Municipal  		
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top">Estado:</td>	
					<td >
						<?php
						$sql_combo = "SELECT
									regcod AS codigo,
									descricaouf AS descricao
								FROM
									public.uf
								WHERE idpais = 1
								ORDER BY 2 ASC;";
						combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
				    	?>		
					</td>
				</tr>
				<tr id="tr_municipio" style="display:none;">
					<td class="SubTituloDireita" valign="top">Município:</td>	
					<td id="td_municipio">
						<?php 
						$sql_combo = "SELECT
										muncod AS codigo,
										regcod ||' - '|| mundsc AS descricao
									FROM
										public.municipio
									Where munstatus = 'A'
									and regcod = 'XX'
									ORDER BY 2 ASC;";
						combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);			
						?>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<input type="button" class="pesquisar" value="Pesquisar"/>
						<div style="float:right">
							
						<input type="button" class="limpar" value="Limpar Pesquisa"/>
						</div>
					</td>
				</tr>
				<tr>
					<td id="td_lista" colspan="2">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>