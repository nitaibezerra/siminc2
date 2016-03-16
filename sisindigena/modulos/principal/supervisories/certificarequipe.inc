<? 

verificarTermoCompromisso(array("iusd"=>$_SESSION['sisindigena']['supervisories']['iusd'],"sis"=>'supervisories',"pflcod"=>PFL_SUPERVISORIES));

if(!$_SESSION['sisindigena']['supervisories']['uncid']) {
	$al = array("alert"=>"Usuário não vinculado com nenhuma universidade","location"=>"sisindigena.php?modulo=inicio&acao=C");
	alertlocation($al);
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

function certificarEquipe() {
	
	document.getElementById('formulario').submit();

}

</script>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Certificar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sisindigena/sisindigena.php?modulo=principal/supervisories/supervisoriesexecucao&acao=A&aba=certificarequipe"); ?></td>
</tr>
<tr>

<td colspan="2">
<form method=post name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="certificarEquipe">
<?

$sql_certificacao = "
SELECT 
		replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,
		i.iusnome,
		p.pfldsc,
		i.iusd
		FROM sisindigena.identificacaousuario i
		INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd
		INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
		WHERE t.pflcod IN('".PFL_ORIENTADORESTUDO."','".PFL_PROFESSORALFABETIZADOR."') AND i.picid='".$_SESSION['sisindigena']['supervisories']['picid']."' AND
			  (i.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd='".$_SESSION['sisindigena']['supervisories']['iusd']."') OR
			   i.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd='".$_SESSION['sisindigena']['supervisories']['iusd']."'))) AND 
			  i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";

$certificandos = $db->carregar($sql_certificacao);

$periodosRef = $db->carregar("SELECT fo.fpbid as codigo, m.mesdsc || '/' || substr(fpbanoreferencia::text,3,2) as descricao FROM sisindigena.folhapagamentouniversidade fu 
							  INNER JOIN sisindigena.folhapagamento fo ON fu.fpbid = fo.fpbid 
							  INNER JOIN public.meses m ON m.mescod::integer = fo.fpbmesreferencia 
							  WHERE fu.uncid='".$_SESSION['sisindigena']['supervisories']['uncid']."' AND picid='".$_SESSION['sisindigena']['supervisories']['picid']."' 
							  ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date");

echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';

if($certificandos[0]) {

	echo '<tr>';
	echo '<td class="SubTituloCentro" style="font-size:x-small;">CPF</td>';
	echo '<td class="SubTituloCentro" style="font-size:x-small;">Nome</td>';
	echo '<td class="SubTituloCentro" style="font-size:x-small;">Perfil</td>';
	
	if($periodosRef[0]) {
		foreach($periodosRef as $pr) {
			echo '<td class="SubTituloCentro" style="font-size:x-small;">'.$pr['descricao'].'</td>';
		}
	}
	
	echo '<td class="SubTituloCentro" style="font-size:x-small;">Total</td>';
	echo '<td class="SubTituloCentro" style="font-size:x-small;">&nbsp;</td>';
	
	echo '</tr>';
	
	foreach($certificandos as $cert) {

		echo '<tr>';
		echo '<td style="font-size:x-small;">'.$cert['iuscpf'].'</td>';
		echo '<td style="font-size:x-small;">'.$cert['iusnome'].'</td>';
		echo '<td style="font-size:x-small;">'.$cert['pfldsc'].'</td>';
		
		$total = 0;
		if($periodosRef[0]) {
			foreach($periodosRef as $pr) {

				$sql = "SELECT mavfrequencia FROM sisindigena.mensario m 
				 		INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = m.menid  
				 		WHERE iusd='".$cert['iusd']."' AND fpbid='".$pr['codigo']."'";
				
				$men = $db->pegaLinha($sql);
				
				switch($men['mavfrequencia']) {
					case '1.0':
						$descricao = '<b style="color:blue">Presença i</b>';
						$total += 1.0;
						break;
					case '0.5':
						$descricao = '<b style="color:green;">Presença p</b>';
						$total += 0.5;
						break;
					case '0.0':
						$descricao = '<b style="color:red;">Ausência</b>';
						break;
					default:
						$descricao = '<b>Não aval</b>';
				}

				echo '<td style="font-size:x-small;">'.$descricao.'</td>';
			}
		}
		
		if($total>10) $total=10;
		
		echo '<td style="font-size:x-small;">'.number_format($total,1).'<input type="hidden" name="total['.$cert['iusd'].']" value="'.number_format($total,1).'"></td>';
		
		$opcao = $db->pegaUm("SELECT ceropcao FROM sisindigena.certificados WHERE iusd='".$cert['iusd']."'");
		
		echo '<td style="font-size:x-small;">';
		echo '<input type="radio" name="certificacao['.$cert['iusd'].']" value="" '.(($opcao)?'':'checked').'> Ainda continua cursando<br>';
		echo '<input type="radio" name="certificacao['.$cert['iusd'].']" value="A" '.(($total>=7.5)?'':'disabled').' '.(($opcao=='A')?'checked':'').'> Concluído e certificado<br>';
		echo '<input type="radio" name="certificacao['.$cert['iusd'].']" value="N" '.(($opcao=='N')?'checked':'').'> Concluído e não certificado';
		echo '</td>';
		
		
		
		echo '</tr>';
	}
	
}

echo '</table>';

?>
</form>
</td>

</tr>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/supervisories/supervisoriesexecucao&acao=A&aba=acompanhamentoavaliacaobolsas';">
	<input type="button" value="Salvar" id="salvar" onclick="divCarregando();certificarEquipe();">
	</td>
</tr>
</table>