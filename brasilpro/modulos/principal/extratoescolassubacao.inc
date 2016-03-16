<?php
cte_verificaSessao();

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>

  </head>
<?
$sql = "SELECT ent.entnome
		FROM cte.instrumentounidadeescola iue
		INNER JOIN entidade.entidade ent on ent.entid = iue.entid
		INNER JOIN cte.qtdfisicoano qtd ON qtd.entid = ent.entid
		WHERE qtd.sbaid = '". $_REQUEST['sbaid'] ."' AND qtd.qfaid = '". $_REQUEST['qfaid'] ."'";
$nome = $db->carregar($sql);
if($nome) {
	$nome = current($nome);
}
?>
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center"><? echo $nome['entnome']; ?></div>
          <div>
			<?php
			$sql = "SELECT cs.cosdsc, und.undddsc, ec.ecsqtd, to_char(cs.cosvlruni, '99999999.99') AS cosvlruni, to_char((ec.ecsqtd*cs.cosvlruni), '99999999.99') AS total 
					FROM cte.composicaosubacao AS cs
						INNER JOIN cte.escolacomposicaosubacao AS ec ON ec.cosid = cs.cosid
						INNER JOIN cte.unidademedidadetalhamento und ON cs.unddid = und.unddid 
					WHERE cs.sbaid = '". $_REQUEST['sbaid'] ."' 
					AND ec.qfaid = '". $_REQUEST['qfaid'] ."'
					AND ecsqtd != 0 
					ORDER BY cs.cosdsc";
			
			$arDados = $db->carregar($sql);
			
			if( $arDados ){
				$somaQtd = 0;
				$somaTotal = 0;
				$arTotal = array();
				foreach( $arDados as $dado ){
					$somaQtd += isset( $dado["ecsqtd"] ) ? $dado["ecsqtd"] : 0;
					//$somaTotal += isset( $dado["total"] ) ? str_replace( ".", "", $dado["total"] ) : 0;
					$arTotal[] = $dado["total"];
				}
				
				$somaTotal = array_sum( $arTotal );
			
		        $arTotais["cosdsc"] = "<div style='text-align: right;' ><b>Total:</b></div>";
		        $arTotais["undddsc"] = ""; 
		        $arTotais["ecsqtd"] = "<div style='text-align: right;' ><b>$somaQtd</b></div>";
		        $arTotais["cosvlruni"] = ""; 
		        $arTotais["total"] = "<div style='text-align: right;' ><b>". number_format( $somaTotal, 2, ',', '.') ."</b></div>";
		        
		        $arDados[] = $arTotais;
			}
			else{
				$arDados = $sql;
			}			
			
			$db->monta_lista_simples($arDados, array('Itens', 'Unidade de Medida', 'Quantidade','Preço Unitário','Total (R$)'), 1000, 1000, 'N', '100%','N');
			?>
          </div>
        </td>
      </tr>
    </table>
</html>
