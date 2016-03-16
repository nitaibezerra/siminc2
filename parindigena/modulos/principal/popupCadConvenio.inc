<?php
#cadastra um convenio

include_once APPRAIZ . "/www/parindigena/_constantes.php";



function verificaTipoPerfil( $usucpf )
{
    global $db;
    $sql = "SELECT pflcod FROM seguranca.perfilusuario WHERE usucpf = '$usucpf'";
    $perfil = $db->pegaUm( $sql );
    return $perfil;    
}
  
$somenteLeitura = 'S';
 global $db;
if ($_REQUEST['covcod']) {
    // query para carregar o convenio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.convenioindigena AS pc
            INNER JOIN financeiro.convenio AS fc ON fc.covcod = pc.covcod
            WHERE
                pc.covcod = '{$_REQUEST['covcod']}'";               
    
    $rsDadosConv = $db->carregar( $sql );
}
//FIM DO INSERT PARA O LABORATORIO.
$convenioAno = $rsDadosConv[0]['covnumero'];
$dadosConvenio = explode('/', $convenioAno);
$numConvenio = $dadosConvenio[0];
$ano = $dadosConvenio[1];
if (array_key_exists('btnSalvar', $_REQUEST ) ) 
{
    
    $coivalorfinicial    = $_REQUEST['coivalorfinicial'];
    $coivalorfcontinuada = $_REQUEST['coivalorfcontinuada'];
    $coivalormaterial    = $_REQUEST['coivalormaterial'];
 
    
    
    $sql = "UPDATE
                parindigena.convenioindigena
                     SET
                         coivalorfinicial    = " . str_replace(',', '.', str_replace('.','', $_REQUEST['coivalorfinicial'])). ",
                         coivalorfcontinuada = " . str_replace(',', '.', str_replace('.','', $_REQUEST['coivalorfcontinuada'])). ", 
                         coivalormaterial    = " . str_replace(',', '.', str_replace('.','', $_REQUEST['coivalormaterial'])). "      
                     WHERE
                     covcod = '{$_REQUEST['covcod']}'";
     
                   
                    $db->executar( $sql );
                    $db->commit();
                    
                    echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';  
}

$sql = "SELECT	 
			si.sbaid
		FROM parindigena.convenioindigena ci
		INNER JOIN financeiro.convenio 		cf ON ci.covcod = cf.covcod
		INNER JOIN cte.projetosape 			ps ON cf.covprocesso::numeric = ps.prsnumeroprocesso
		INNER JOIN cte.projetosapesubacao 	psi ON psi.prsid = ps.prsid
		inner join cte.subacaoindicador 	si ON si.sbaid = psi.sbaid
		INNER JOIN cte.instrumentounidade 	iu ON iu.inuid = ps.inuid 
		WHERE ci.coistatus = 'A'
		AND ci.covcod 	= '{$_REQUEST['covcod']}'
		--AND cf.estuf 	= '{$_REQUEST['estuf']}' 
		--AND iu.estuf 	= '{$_REQUEST['estuf']}'";
$arSubacoes = $db->carregar($sql);	

if(is_array($arSubacoes)){
	foreach($arSubacoes as $dados){
		$arSbaids[] = $dados['sbaid'];	
	}

$sql = "SELECT 
			sum(total) AS total,
			apptipo		as apftipo	
		FROM (
		
			SELECT 
				somaquantidadeitens			AS quantidade,
				cos.cosvlruni 				AS valorunitario,
				apf.apptipo,
				somaquantidadeitens*cos.cosvlruni 	AS total
			FROM cte.subacaoindicador sba
			INNER JOIN cte.composicaosubacao 	cos ON sba.sbaid = cos.sbaid AND cosano >= '$ano'
			INNER JOIN cte.qtdfisicoano 		qtd ON sba.sbaid = qtd.sbaid AND qtd.qfaano >= '$ano'
			INNER JOIN parindigena.associativaplanodemetasprogramas apf ON apf.sbaid = sba.sbaid	
			LEFT JOIN (
			
					SELECT cosid, SUM(ecsqtd) as somaquantidadeitens 
					FROM cte.escolacomposicaosubacao 
					GROUP BY cosid
					
				  ) ecs ON cos.cosid = ecs.cosid
			WHERE sba.sbaid IN ('".implode("', '",$arSbaids)."') 
			AND sba.sbaporescola = 't'
			--AND apf.apftipo = '1'
			
			UNION ALL
		
			SELECT 
				cos.cosqtd 			AS quantidade,
				cos.cosvlruni		AS valorunitario,
				apf.apptipo,
				cos.cosqtd*cos.cosvlruni 	AS toatal
			FROM cte.subacaoindicador sba
			INNER JOIN cte.composicaosubacao 	cos ON sba.sbaid = cos.sbaid AND cosano >= '$ano'
			INNER JOIN parindigena.associativaplanodemetasprogramas apf ON apf.sbaid = sba.sbaid
			WHERE sba.sbaid IN ('".implode("', '",$arSbaids)."') 
			AND sba.sbaporescola = 'f'
			--AND apf.apftipo = '1'
		
		) AS totais
		GROUP BY apptipo 
		ORDER BY apptipo asc";

$arTotais = $db->carregar($sql);

}
$arTotais = $arTotais ? $arTotais : array();
if(is_array($arTotais)){
	foreach($arTotais as $valores){
		if($valores['apftipo'] == 1){
			$valorTipo1 = $valores['total'] != '' ? $valores['total'] : "0";
		}else if($valores['apftipo'] == 2){
			$valorTipo2 = $valores['total'] != '' ? $valores['total'] : "0";
		}
		else if($valores['apftipo'] == 3){
			$valorTipo3 = $valores['total'] != '' ? $valores['total'] : "0";
		}
	}
}

    ?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades-dev.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
    </style>
  </head>
  <body onLoad="bloquearCampos();">

<form onsubmit=" return validaConvenio();" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario" >
    <table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Identificaçao do Convênio</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">

             <tr>
                <td class="subtitulodireita" >Número do Convênio:</td>
                <td align="left">
                <?php $covnumero = $rsDadosConv[0]['covnumero']; echo $covnumero; ?>
                <?
                # campo_texto('covnumero', 'N', $somenteLeitura, '', 20, 18, '', '', 'left', '',  0, 'id="covnumero" onblur="MouseBlur(this);"' ); 
                ?>
                </td>
             </tr>   
            
          
             <tr>
                <td class="subtitulodireita">Objeto do Convênio:</td>
                <td  align="left">
                <?php $covobjeto = $rsDadosConv[0]['covobjeto']; echo $covobjeto; ?>
                <? #campo_texto('covobjeto', 'N', $somenteLeitura, '', 80, 18, '', '', 'left', '',  0, 'id="covobjeto" onblur="MouseBlur(this);"' ); 
                ?>
                </td>
             </tr>  
             <tr>
                <td class="subtitulodireita"> Detalhamento do Convênio:</td>
                <td align="left">
                    <!--<TEXTAREA COLS="77" ROWS="6" NAME="covdetalhamento" id ="covdetalhamento" >--><?php $covdetalhamento = $rsDadosConv[0]['covdetalhamento']; echo("$covdetalhamento"); ?><!--</TEXTAREA>-->
                </td>
                    
             </tr>
             
             <tr>
                <td class="subtitulodireita">Processo:</td>
                <td align="left">
                 <?php $covprocesso = $rsDadosConv[0]['covprocesso']; echo $covprocesso; ?>
                <? #campo_texto('covprocesso', 'N', $somenteLeitura, '', 30, 38,'', '', 'left', '',  0, 'id="covprocesso" onblur="MouseBlur(this);"' );
                ?>
                </td>
             </tr>   
             <tr>
                <td class="subtitulodireita">Valor do Convênio:</td>
                <td align="left">
                 <?php
                // $covvalor = $rsDadosConv[0]['covvalor']; 
                 $covvalor = number_format($rsDadosConv[0]['covvalor'],2,',','.'); 
                 echo 'R$ '.$covvalor; 
                 ?>
                 </td>
             </tr>   
             <tr>
                <td class="subtitulodireita" >Valor do Concedente:</td>
                <td align="left">
                <?php 
                
                   $covconcedente = number_format($rsDadosConv[0]['covconcedente'],2,',','.'); 
                 echo 'R$ '.$covconcedente; 
                 ?>
                </td>
             </tr>   
             <tr>
                <td class="subtitulodireita">Valor do Convenente:</td>
                <td align="left">
                
                     <?php 
                
                   $covconvenente = number_format($rsDadosConv[0]['covconvenente'],2,',','.'); 
                 echo 'R$ '.$covconvenente; 
                 ?>
               
                
                </td>
             </tr>  
             <tr>
                <td colspan="2" align='left' class="SubTituloDireita"><div class="SubtituloEsquerda" style="padding: 5px; text-align: left">Valores do convênio por tipo</td>
            </tr> 
           
            
              <tr>
                <td class="subtitulodireita">Formaçao Inicial:</td>
                <td align="left">
                 <?php
                 //$coivalorfinicial = number_format($rsDadosConv[0]['coivalorfinicial'],2,',','.'); 
                 $coivalorfinicial = $valorTipo1;
                 $coivalorfinicial = number_format($coivalorfinicial,2,',','.'); 
                 ?>
                 <?= campo_texto('coivalorfinicial', 'N', 'N', '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="coivalorfinicial" onblur="MouseBlur(this);"' ); ?>
  
                </td>
             </tr>   
             <tr>
                <td class="subtitulodireita" >Formaçao Continuada:</td>
                <td align="left">
                 <?php
                 //$coivalorfcontinuada = number_format($rsDadosConv[0]['coivalorfcontinuada'],2,',','.');
                 $coivalorfcontinuada = $valorTipo2; 
                 $coivalorfcontinuada = number_format($coivalorfcontinuada,2,',','.'); 
                 ?>
                 <?= campo_texto('coivalorfcontinuada', 'N', 'N', '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="coivalorfcontinuada" onblur="MouseBlur(this);"' ); ?>
  
                </td>
             </tr>   
             <tr>
                <td class="subtitulodireita">Material Didático:</td>
                <td align="left">
                <?php
                 //$coivalormaterial = number_format($rsDadosConv[0]['coivalormaterial'],2,',','.');
                 $coivalormaterial = $valorTipo3; 
                 $coivalormaterial = number_format($coivalormaterial,2,',','.'); 
                 ?>
                <?= campo_texto('coivalormaterial', 'N', 'N', '', 30, 100, '#.###.###.###,##', '', 'left', '',  0, 'id="coivalormaterial" onblur="MouseBlur(this);"' ); ?>
  
                 
                </td>
             </tr>  

            <tr style="background-color: #cccccc;">
                 <td class="SubTituloEsquerda" colspan="2" align="left">
                 <?php
                 
                    $usucpf = $_SESSION['usucpf'];
                    $perfil = verificaTipoPerfil( $usucpf );
                    if( ( PARIND_COORDENACAO == $perfil ) || ( PARIND_SUPER_USUARIO == $perfil ) )
                    {  
                          $disable = '';
                    }   
                    else
                    {
                         $disable = "disabled = \"true\"";
                    }
                        ?>
                        <input type="submit" name="btnSalvar" value="Salvar" <?php if( $disable != '' ){ echo $disable; } ?>/>
                       <input type="button" name="btnCancel" value="Fechar" onClick="fecharJanela();"/>
                 </td>
            </tr>
                  
                </table>
          </form>
  </body>

  <script type="text/javascript">
  function fecharJanela()
  {
      return window.close();
  }
 
  
  function validaConvenio()
  {
      var coivalorfinicial      = document.getElementById('coivalorfinicial').value;
      var coivalorfcontinuada   = document.getElementById('coivalorfcontinuada').value;
      var coivalormaterial      = document.getElementById('coivalormaterial').value;
      
      var msg = 'É necessário preencher todos campos.';
  
      if ( coivalorfinicial < 1 )
      {
          alert( msg );
          return false;
      }

      if ( coivalorfcontinuada < 1 )
      {
          alert( msg );
          return false;
      }

      if ( coivalormaterial < 1 )
      {
          alert( msg );
          return false;
      }      
      
  }
  
  
  function bloquearCampos()
  {
     var numForm = $('formulario').elements.length;
      
     if( numForm > 0) {
         for(i=0; i< 7 ; i++) 
         {
              
             // formulario.elements[i].disabled = 1;
         }
     }
  }

  $('coivalorfinicial').value = mascaraglobal('#.###.###.###,##', $('coivalorfinicial').value );
  $('coivalorfcontinuada').value = mascaraglobal('#.###.###.###,##', $('coivalorfcontinuada').value );
  $('coivalormaterial').value = mascaraglobal('#.###.###.###,##', $('coivalormaterial').value );
  </script>
</html>



<?php                        

?>

