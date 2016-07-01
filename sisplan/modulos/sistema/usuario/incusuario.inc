<?php
/** 
 * Desenvolvedor: MESOTEC Informática LTDA
 * Analistas: Adonias Malosso <adonias@mesotec.com.br>, Davison Silva <davison@mesotec.com.br>
 * Programadores: Adonias Malosso <adonias@mesotec.com.br>, Halisson Gomides <halisson@mesotec.com.br>, Paulo Estevao <paulo@mesotec.com.br>
 * Baseado no SIMEC - cedido pela DTI/SE/MEC como software público brasileiro
 */
	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php
			$sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflsuporte = 'f' and pflstatus='A' and seguranca.perfil.pflnivel > 1 and sisid=". $sisid ." order by descricao";
			$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", '', '', '', '', 'S' );
		?>
	</td>
</tr>