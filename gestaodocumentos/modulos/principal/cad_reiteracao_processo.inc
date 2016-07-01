<?php
//	ver($_REQUEST,d);

    if($_REQUEST['taridprincipal'] == ''){
        echo "<script type=\"text/javascript\">
                	alert('Não é possível cadastar as retificações, tente novamente mais tarde!');
                	window.close();
              </script>";
        exit();
    } else {
        $taridprincipal = $_REQUEST['taridprincipal'];
    }

    require_once APPRAIZ . "gestaodocumentos/classes/Reiteracao.class.inc";

    function salvarReiteracao($dados){
        global $db;
        $objReiteracao = new Reiteracao();
        if(isset($dados['tarid'])){
            foreach($dados['tarid'] as $taridsecundario){
                $arDados['taridprincipal']  = $dados['taridprincipal'];
                $arDados['taridsecundario'] = $taridsecundario;
                $objReiteracao->popularDadosObjeto( $arDados )-> salvar();
                $objReiteracao->clearDados();
            }
            $db->commit();
        }
        $db->sucesso('principal/cadTarefa', '&tarid='.$dados['taridprincipal'],'Operação Realizada com Sucesso!','S','S');
    }

    function filtrarListaProcesso(){
    	global $db;
        header('content-type: text/html; charset=ISO-8859-1');
        extract($_POST);

        if(trim($tipo_pesquisa) == 'filtro'){
        	$tardatarecebimento_ini = formata_data_sql($tardatarecebimento_ini);
            $tardatarecebimento_fim = formata_data_sql($tardatarecebimento_fim);

            if($tarid){
            	$aryWhere[] = "t.tarid = '{$tarid}'";
            }

            if($tarnumsidoc){
                $aryWhere[] = "t.tarnumsidoc ILIKE ('%{$tarnumsidoc}%')";
            }

            if($tartitulo){
               $aryWhere[] = "t.tartitulo ILIKE ('%". trim($tartitulo). "%')";
            }

            if($tarnumidentexterno){
               $aryWhere[] = "t.tarnumidentexterno ILIKE ('%". trim($tarnumidentexterno). "%')";
            }

            if($tarnumprocexterno){
               $aryWhere[] = "t.tarnumprocexterno ILIKE ('%". trim($tarnumprocexterno). "%')";
            }

            if($tardatarecebimento_ini && $tardatarecebimento_fim) {
               $aryWhere[] = "t.tardatarecebimento BETWEEN '{$tardatarecebimento_ini}' AND '{$tardatarecebimento_fim}'";
            }
        }

        $taridprincipal = $_REQUEST['taridprincipal'] ? $_REQUEST['taridprincipal'] : $taridprincipal;

		$aryWhere[] = "tarstatus = 'A'";
        $aryWhere[] = "_tarpai IS NULL";
        $aryWhere[] = "t.tarid <> {$taridprincipal}";

        $sql = "SELECT  '<input type=\"checkbox\" name=\"tarid[]\" id=\"tarid\" value=\"'||t.tarid||'\">' as acao,
                        TRIM(array_to_string(
                             array(
                                   SELECT
                                   	      		CASE     WHEN iss.iesidinstituicaoensino IS NOT NULL THEN '- '|| UPPER(ie.iesdsc)
                                            	  	     WHEN iss.uamid IS NOT NULL THEN '- '|| UPPER(u.uamdsc)
                                                		 WHEN iss.ogsid IS NOT NULL THEN '- '|| UPPER(og.ogsdsc)
                                                  		 WHEN iss.solidpessoafisica IS NOT NULL THEN '- '|| UPPER(s.solnome)
                                          		END AS descricao
                                   FROM 		gestaodocumentos.instituicaosolicitante AS iss
                                   INNER JOIN 	gestaodocumentos.tiposolicitante AS ts ON ts.tpsid = iss.tpsid
                                   LEFT JOIN 	gestaodocumentos.instituicaoensino AS ie ON ie.iesid = iss.iesidinstituicaoensino --INSTITUIÇÃO DE ENSINO
                                   LEFT JOIN 	public.unidadeareamec AS u  ON u.uamid = iss.uamid --ÁREA MEC
                                   LEFT JOIN 	gestaodocumentos.orgaosolicitante AS og ON og.ogsid = iss.ogsid --ÓRGÃO
                                   LEFT JOIN 	gestaodocumentos.solicitantepessoa AS s ON s.solid = iss.solidpessoafisica --SOLICITANTE
                                   WHERE 		iss.tarid = t.tarid
                                   ORDER BY 	descricao), '<br>'
                         ) ) AS solicitante,
                         t.tarnumsidoc,
                         to_char(t.tardatarecebimento, 'DD/MM/YYYY') as tardatarecebimento,
                         t.tartitulo,
                         '<span style=\"color: #1E90FF\">'|| t.tarnumidentexterno ||'</span>' as tarnumidentexterno,
                         '<span style=\"color: #1E90FF\">'|| t.tarnumprocexterno ||'</span>' as tarnumprocexterno
             	FROM 	 gestaodocumentos.tarefa t
                         ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";

        $cabecalho = array( "Ação", "Solicitante", "Nº SIDOC/EMEC", "Recebimento", "Identificação", "Identificação Externa: <br> Nº do Documento", "Identificação Externa: <br> Nº do Processo" );
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', 'N', 'formulario_reiteracao', '', $alinhamento, '', '');
    }

    if($_REQUEST['requisicao'] != '') {
        $_REQUEST['requisicao']($_REQUEST);
        die();
    }
?>

<html>
    <head>
        <title>Cadastro de Reiteração de Processo</title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
        <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <script language="javascript" type="text/javascript">
        	jQuery.noConflict();

    	    function salvarReiteracao(){
                var valid;
                valid = validarCampos();
                if(valid){
            		jQuery('#formulario_reiteracao').children(':first').after('<input type=hidden name=requisicao value=salvarReiteracao />');
                	jQuery('#formulario_reiteracao').submit();
                } else {
                    alert('É necesssário selecionar pelo menos "UM"!');
                }
            }

            function filtrarListaProcesso(param){
            	jQuery('#requisicao').val('filtrarListaProcesso');
                if(trim(param) == 'filtro'){
                	jQuery('#tipo_pesquisa').val('filtro');
                } else {
                	jQuery('#tipo_pesquisa').val('todos');
                }
                divCarregando();
        		jQuery.ajax({
        			type: 'POST',
        			url: 'gestaodocumentos.php?modulo=principal/cad_reiteracao_processo&acao=A',
        			data: jQuery('#formulario').serialize(),
        			async: false,
        			success: function(data){
        				jQuery("#tabela_lista").html(data);
        				divCarregado();
        		   	}
        		});
            }

            function validarCampos(){
                var checado = 'N';
                jQuery('input:checkbox[name=tarid[]]').each(function(){
                    if ( jQuery(this).is(':checked') ){
                        checado = 'S';
                    }
                });

                if(checado === 'S'){
                    return true;
                } else {
                    return false;
                }
            }
        </script>
    </head>

    <body>
        <form name="formulario" id="formulario" method="POST">
            <input type="hidden" name="tipo_pesquisa" id="tipo_pesquisa" value="">
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <input type="hidden" name="taridprincipal" id="taridprincipal" value="<?php echo $taridprincipal;?>">
            <table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
                <tr>
                    <td bgcolor="#CCCCCC" colspan="2">
                        <b>Argumentos da Pesquisa</b>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Número do SIDOC/EMEC:</td>
                    <td>
                        <?php
                        $tarnumsidoc = $_REQUEST['tarnumsidoc'];
                        echo campo_texto('tarnumsidoc', 'N', 'S', '', 42, 25, '', '', 'left', '', 0, 'id="tarnumsidoc"', '', $tarnumsidoc); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Identificação da Demanda:</td>
                    <td>
                        <?php
                        $tartitulo = $_REQUEST['tartitulo'];
                        echo campo_texto('tartitulo', 'N', 'S', '', 42, 25, '', '', 'left', '', 0, '', 'id="tartitulo"', '', $tartitulo); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Identificação Externa - Nº do Documento:</td>
                    <td>
                        <?php
                        $tarnumidentexterno = $_REQUEST['tarnumidentexterno'];
                        echo campo_texto('tarnumidentexterno', 'N', 'S', '', 42, 25, '', '', 'left', '', 0, '', 'id="tarnumidentexterno"', '', $tarnumidentexterno); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Identificação Externa - Nº do Processo:</td>
                    <td>
                        <?php
                        $tarnumprocexterno = $_REQUEST['tarnumprocexterno'];
                        echo campo_texto('tarnumprocexterno', 'N', 'S', '', 42, 25, '', '', 'left', '', 0, '', 'id="tarnumprocexterno"', '', $tarnumprocexterno); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Data de Recebimento do Processo :</td>
                    <td>
                        <?php
                        $tardatarecebimento_ini = $_REQUEST['tardatarecebimento_ini'];
                        $tardatarecebimento_fim = $_REQUEST['tardatarecebimento_fim'];

                        echo "Início " . campo_data2('tardatarecebimento_ini','N','S','','DD/MM/YYYY','','', $tardatarecebimento_ini, '', '', 'tardatarecebimento_ini' ) . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
                        echo "Fim " . campo_data2('tardatarecebimento_fim','N','S','','DD/MM/YYYY','','', $tardatarecebimento_fim, '', '', 'tardatarecebimento_fim' ); ?>
                    </td>
                </tr>
                <tr>
                    <td bgcolor="#CCCCCC" colspan="2" align="center">
                        <input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="filtrarListaProcesso('filtro');">
                        <input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="filtrarListaProcesso('todos');">
                    </td>
                </tr>
            </table>
		</form>

        <br>

        <div id="tabela_lista">
            <?php filtrarListaProcesso(); ?>
		</div>

		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    		<tr>
        		<td bgcolor="#CCCCCC" align="center">
            		<input type="button" name="salvar" id="salvar" onclick="salvarReiteracao();" value="Salvar" style="cursor:pointer;">
           		</td>
           	</tr>
   		</table>
   		<script language="javascript" type="text/javascript">
       		jQuery('#formulario_reiteracao').append('<input id="requisicao" type="hidden" name="requisicao" value="salvarReiteracao" />');
       		jQuery('#formulario_reiteracao').append('<input id="taridprincipal" type="hidden" name="taridprincipal" value="<?php echo $taridprincipal;?>" />');
   		</script>
    </body>
</html>