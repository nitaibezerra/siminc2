<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<?php

if($_REQUEST['atualizarerro']) {
$sql = "select * from sisfor.cursista where length(curnome)=11 and (curnome ~ '^[0-9]+$')=true";
$ar = $db->carregar($sql);

foreach($ar as $a) {
    $teste = recuperarUsuarioReceita(str_pad($a['curnome'], 11, "0", STR_PAD_LEFT));
    echo "UPDATE sisfor.cursista SET curcpf='".str_pad($a['curnome'], 11, "0", STR_PAD_LEFT)."', curnome='".$teste[dados][no_pessoa_rf]."' WHERE curid='".$a['curid']."';<br>";
}
exit;
}

//verificarFluxoComposicaoEquipe($_SESSION['sisfor']['docidcomposicaoequipe']);

if ($_REQUEST['fpbid']) {

    if ($_FILES['arquivo'] != NULL) {
    	
    	/* configurações */
    	ini_set("memory_limit", "2048M");
    	set_time_limit(600);
    	/* FIM configurações */
    	   

        $arquivo = fopen($_FILES['arquivo']["tmp_name"], "r");

        $sql = '';
        $certos = 0;
        $errados = 0;
        $sql = "select sifcpfobrigatorio from sisfor.sisfor sis where sis.sifid = '{$_SESSION['sisfor']['sifid']}'";
        $cpfobrigatorio = $db->pegaUm($sql);
        
        while (($data = fgetcsv($arquivo, 500, ";")) !== FALSE) {
        	$linhaerro++;
            if (count($data) == 5) {

                $cpfAvaliador = trim($data[4]);
                $cpfAvaliador = str_replace(array('.', '-'), array('', ''), $cpfAvaliador);
				
                $cpf = trim($data[0]);
                $cpf = str_replace(array('.', '-'), array('', ''), $cpf);
                
                if(is_numeric($cpfAvaliador)) {
                
	                $sqlOrientador1 = "select iu.iusd as codigo
		                        from sisfor.identificacaousuario iu
		                        where iu.iuscpf='" .str_pad( $cpfAvaliador, 11, "0", STR_PAD_LEFT)."'";
	                $iusidavaliador = $db->pegaUm($sqlOrientador1);
                
                }
	                
                if ($cpfobrigatorio == "t") {
                	
                		if(is_numeric($cpf)) {
	
		                    $teste = recuperarUsuarioReceita(str_pad($cpf, 11, "0", STR_PAD_LEFT));
		                    
		                    if ($teste[usuarioexiste] == 1) {//2
		
		                        
		                        if ($iusidavaliador) {//1
		                        	
		                        	$curId = $db->pegaUm("SELECT curid FROM sisfor.cursista WHERe curcpf='".str_replace(array('.','-','/'),array('','',''),str_pad( $data[0], 11, "0", STR_PAD_LEFT))."'");
		                        	
		                        	if(!$curId) {
			                            $sql = " INSERT INTO sisfor.cursista (curcpf, curnome, curemail) "
			                                    . " VALUES ('" .trim(str_replace(array('.','-','/'),array('','',''),str_pad( $data[0], 11, "0", STR_PAD_LEFT))) . "','" . $teste[dados][no_pessoa_rf] . "', '" . pg_escape_string($data[1]) . "' ) RETURNING  curid; ";
			                            $curId = $db->pegaUm($sql);
		                        	}
		                        	
		                        	$cucId = $db->pegaUm("SELECT cucid FROM sisfor.cursistacurso WHERE curid='".$curId."' AND fpbid='".$_REQUEST['fpbid']."' AND sifid='".$_SESSION['sisfor']['sifid']."'");
		                        	
		                        	if(!$cucId) {
		                        		
		                        		$muncod_validado = $db->pegaUm("SELECT muncod FROM territorios.municipio WHERE muncod='".$data[3]."'");
		                        	
			                            $sql2 = " INSERT INTO sisfor.cursistacurso (curid, fpbid, sifid,cucstatus, iusdavaliador, estuf, muncod) "
			                                    . " VALUES ('" . $curId . "','" . $_REQUEST['fpbid'] . "', '" . $_SESSION['sisfor']['sifid'] . "' , 'A', " . $iusidavaliador . ", '" . $data[2] . "', " . (($muncod_validado)?"'".$muncod_validado."'":"NULL") . "  ); ";
			
			                            $db->executar($sql2);
		                            
		                        	}
		                        	
		                            $db->commit();
		                            
		                            $certos++;
		                            
		                        } else {//1
		                        	$errodt[] = 'Linha '.$linhaerro.' : Avaliador não identificado ('.str_pad( $cpfAvaliador, 11, "0", STR_PAD_LEFT).')';
		                            $errados++;
		                        }
		                        
		                    } else {//2
		                    	$errodt[] = 'Linha '.$linhaerro.' : CPF do cursista não identificado na Receita Federal ('.$cpf.')';
		                        $errados++;
		                    }
	                    
                		} else {
                			$errodt[] = 'Linha '.$linhaerro.' : CPF do cursista não númerico ('.$cpf.')';
                			$errados++;
                		}
	                } else {//3
	                	
	                        if ($iusidavaliador) {
	                     
	                            $sql = " INSERT INTO sisfor.cursista (curcpf, curnome, curemail) "
	                                    . " VALUES (NULL,'" . pg_escape_string(str_replace(array('.','-','/'),array('','',''),str_pad( $data[0], 11, "0", STR_PAD_LEFT))) . "', '" . pg_escape_string($data[1]) . "' ) RETURNING  curid; ";
	                          
	                            $curId = $db->pegaUm($sql);
	                            $sql2 = " INSERT INTO sisfor.cursistacurso (curid, fpbid, sifid,cucstatus, iusdavaliador, estuf, muncod) "
	                                    . " VALUES ('" . $curId . "','" . $_REQUEST['fpbid'] . "', '" . $_SESSION['sisfor']['sifid'] . "' , 'A', " . $iusidavaliador . ", '" . $data[2] . "', '" . $data[3] . "'  ); ";
	                          
	                            $db->executar($sql2);
	                            $db->commit();
	                            $certos++;
	                        } else {
	                        	$errodt[] = 'Linha '.$linhaerro.' : Avaliador não identificado ('.str_pad( $cpfAvaliador, 11, "0", STR_PAD_LEFT).')';
	                            $errados++;
	                        }
	                }
                

            } else {
                echo '<script>'
                . 'alert("Formato inválido! \nO arquivo .csv deve ter 5 colunas, sendo elas: \n cpf do cursista, E-mail, Estado, Município, Cpf do avaliador.")'
                . '</script>';
                $validador_insert = false;
                $sql = '';
                break;
            }
        }
        
        echo '<script>' .
        "alert('$certos cursista(s) importados com sucesso e $errados tiveram problema na importação!".(($errodt)?'\n'.implode('\n',$errodt):'')."');
        window.location='sisfor.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=cadastrar_cursista&fpbid=".$_REQUEST['fpbid']."';</script>";
        exit;
    } else {
        if ($_FILES['arquivo'] != NULL) {
            echo '<script type="text/javascript">'
            . 'alert("Formato inválido! \nA extensão do arquivo deve ser .csv com os valores separados por ponto-e-vírgula(;).")'
        	. 'window.location=\'sisfor.php?modulo='.$_REQUEST['modulo'].'&acao=A&aba=cadastrar_cursista&fpbid='.$_REQUEST['fpbid'].'\';'
            . '</script>';
            exit;
        }
        
    }
}


monta_titulo('Importar Cursistas', '');
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" class="SubTituloCentro">
<?
carregarPeriodoReferenciaCursista(array('sifid' => $_SESSION['sisfor']['sifid'], 'fpbid' => $_REQUEST['fpbid']));
?>
        </td>
    </tr>
</table>


<?php
if (!empty($_GET['fpbid'])) {



    $sql = "select sifcpfobrigatorio from sisfor.sisfor sis where sis.sifid = '{$_SESSION['sisfor']['sifid']}'";

    $cpfobrigatorio = $db->pegaUm($sql);
    ?>






    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <form action="" method="post" name="formulario_import" id="formulario_import" enctype="multipart/form-data">
                    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                        <tr>
                            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                             A planilha para importação deverá seguir o seguinte formato e salva com a extensão .csv (valores separados por ponto-e-vírgula ';' ).
                            </td>
                            <td>
                                <table style="border: none; width: 100%">
                                    <tr>
    <?php if ($cpfobrigatorio == "f") { ?>
                                            <td style="border: 1px #000 solid;">Nome do cursista</td>
    <?php } else { ?>
                                            <td style="border: 1px #000 solid;">CPF do cursista</td>
                                        <?php } ?>
                                        <td style="border: 1px #000 solid;">E-mail</td>
                                        <td style="border: 1px #000 solid;">Sigla do Estado</td>    
                                        <td style="border: 1px #000 solid;">Código IBGE do município<br> ( <span style="font-size:x-small;"><a target="blanck" href="http://www.ibge.gov.br/home/geociencias/areaterritorial/area.shtm">Veja aqui a tabela de municípios no site do ibge</a> )</span></td>
                                        <td style="border: 1px #000 solid;">CPF do avaliador</td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                        <tr id="tr_botoes_acao">
                            <td class="SubTituloDireita">
                                Os CPF's dos avaliadores que podem ser usados no arquivo .cvs são:
                            </td>
                            <td>
                                <div style="height:200px;overflow:auto;">
    <?php
    $sqlOrientador = "select iu.iusd as codigo,replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf, iusnome as descricao
						                        from sisfor.identificacaousuario iu
						                            inner join sisfor.tipoperfil tp on tp.iusd = iu.iusd
						                        where sifid = '{$_SESSION['sisfor']['sifid']}'
						                        order by descricao";

    $dadosOrientador = $db->carregar($sqlOrientador);
    foreach ($dadosOrientador as $value) {
        echo "<br>";
        echo "<b>  " . $value['cpf'] . " </b>- $value[descricao]";
    }
    ?>
                                </div>
                            </td>
                        </tr>


                        <tr>
                            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;"><label for="arquivo">Arquivo:</label></td>
                            <td> <input type="file" name="arquivo" id="arquivo"> </td>
                        </tr>

                        <tr style="background-color: #cccccc">
                            <td colspan=2>
                                <input type="button" name="botao_import" id="botao_import" value="Importar" />
                            </td>
                        </tr>
                    </table>
<?php } ?>
            </form>
            <script type="text/javascript">
                
                
                
                
                jQuery(function() {

                    jQuery('#botao_import').click(function() {
                        //jQuery('#formulario_gravar').submit();            

                        var resp = confirm("Atenção! Este arquivo deve estar exatamente no formado ilustrado nessa página! Deseja continuar?")
                        if (resp == true) {
                            if(jQuery('#arquivo').val()=='') {
								alert('Selecione o arquivo CSV');
								return false;
                            }
                            jQuery('#formulario_import').submit();
                        } else {
                            alert('Importação cancelada!');
                        }
                    });
                });
            </script>
        </td>
    </tr>
</table>



