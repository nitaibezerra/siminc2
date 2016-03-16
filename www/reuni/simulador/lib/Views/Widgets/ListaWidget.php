<?php
/*
 * Created on 09/09/2007 by MOC
 *
 */

class ListaWidget{
	private $view;
	private $cssclass='lista';
	private $headers;
	private $rows;
	private $postname;
	private $postcol;

    public function __construct($view,$cssclass,$headers,$postname,$postcol,$rows) {
    	$this->view = $view;
    	$this->cssclass = $cssclass;
    	$this->headers = $headers;
    	$this->rows = $rows;
    	$this->postname = $postname;
    	$this->postcol = $postcol;
    }

    public function display(){
		$output='
		<BR>
		<div id="lista">
		<a class="legenda"><img ALIGN=ABSMIDDLE src="imagens/b_new.png">= Incluir
		<img ALIGN=ABSMIDDLE src="imagens/b_edit.png">= Editar
		<img ALIGN=ABSMIDDLE src="imagens/b_delete.png">= Excluir';
		if ($this->view=='graduacao') {
			$output.='<img ALIGN=ABSMIDDLE src="imagens/b_vagas.png">= Vagas e Concluintes ';
			$output.='<img ALIGN=ABSMIDDLE src="imagens/b_vagas2.png">= Vagas Totais 2002-2005';
		}
		if ($this->view=='pos_graduacao') $output.='<img ALIGN=ABSMIDDLE src="imagens/b_vagas.png">= Matriculados';
		$output.='</a>';
		$output.='<table class="'.$this->cssclass.'" width="800" align="center">
		<tr>';
		foreach ($this->headers as $key => $value) {
			$output.='<th class="center">'.$key.'</th>';
		}

		if ($this->view=='unidades') {
		$output.='
			<th class="center" colspan="2"><a href="index.php?view='.$this->view.'&action=incluir"><img ALIGN="middle" src="imagens/b_new.png" title="Incluir"></a></th>
		</tr>';
		}

		if ($this->view=='pos_graduacao') {
		$output.='
			<th class="center" colspan="3"><a href="index.php?view='.$this->view.'&action=incluir"><img ALIGN="middle" src="imagens/b_new.png" title="Incluir"></a></th>
		</tr>';
		}

		if ($this->view=='graduacao') {
		$output.='
			<th class="center" colspan="2"><a href="index.php?view='.$this->view.'&action=incluir"><img ALIGN="middle" src="imagens/b_new.png" title="Incluir"></a></th>
			<th class="center" colspan="1"><a href="index.php?view='.$this->view.'&action=vagastcg"><img ALIGN="middle" src="imagens/b_vagas2.png" title="Vagas 2002-2005"></a></th>
		</tr>';
		}
		if ($this->rows) {
			foreach ($this->rows as $row) {
				$output.='
				<tr>';
					foreach ($this->headers as $key => $value) {
						if ($key==='Duração') {
							$output.='<td STYLE="text-align:right">'.number_format($row[$value],1,',','.').'</td>';
						} else
							$output.='<td>'.$row[$value].'</td>';
					}
				$output.='
					<td width="1%" class="center"><a href="index.php?view='.$this->view.'&action=alterar&'.$this->postname.'='.$row[$this->postcol].'"><img ALIGN="middle" src="imagens/b_edit.png" title="Alterar"></a></td>
					<td width="1%" class="center"><a href="index.php?view='.$this->view.'&action=excluir&'.$this->postname.'='.$row[$this->postcol].'"><img ALIGN="middle" src="imagens/b_delete.png" title="Excluir"></a></td>';
					if ($this->view=='graduacao') $output.='<td width="1%" class="center"><a href="index.php?view='.$this->view.'&action=vagas&'.$this->postname.'='.$row[$this->postcol].'"><img ALIGN="middle" src="imagens/b_vagas.png" title="Vagas e Concluintes"></a></td>';
					if ($this->view=='pos_graduacao') $output.='<td width="1%" class="center"><a href="index.php?view='.$this->view.'&action=vagas&'.$this->postname.'='.$row[$this->postcol].'"><img ALIGN="middle" src="imagens/b_vagas.png" title="Matriculados"></a></td>';
				$output.='
				</tr>
				';
			}
		}
		$output.= '
		</table>
		</div>
		<BR>
		';
		return $output;
    }
}

?>
